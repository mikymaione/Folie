/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Ball.hpp"
#include "REF.hpp"

Folie::Ball::Ball()
{
	waiter = gcnew CoroutineQueue();
}

// Unity
void Folie::Ball::Start()
{
	rigidBody = GetComponent<UnityEngine::Rigidbody^>();
	_started = true;
}

// Unity
void Folie::Ball::Update()
{
	if (inHand != nullptr)
		transform->SetPositionAndRotation(inHand->position, inHand->rotation);

	if (transform->position.y < -0.3 || GB::outOfTheVisibleArea3D(transform->position))
	{
		ballOnTheFloor();

		rigidBody->velocity = UnityEngine::Vector3::zero;
		transform->position = UnityEngine::Vector3(11.69f, 0.4f, 7.93f);
	}
}

void Folie::Ball::OnCollisionEnter(UnityEngine::Collision collision)
{
	auto gobj = collision.collider->gameObject;

	if (gobj->CompareTag("Player"))
		lastPlayerTouch = gobj->GetComponent<Player ^>();
	else if (hitted && gobj->CompareTag("Pavimento"))
		ballOnTheFloor();
}

void Folie::Ball::attachToHand(String ^player_name)
{
	for each (auto team in REF::teams)
		for each (auto p in team->players)
			if (p->name->Equals(player_name))
			{
				inHand = p->hand;
				REF::game->playerName->text = "Last touch: " + p->name;

				hitted = false;
				ground = false;

				break;
			}
}

bool Folie::Ball::ballInHand()
{
	return inHand != nullptr;
}

bool Folie::Ball::ballIsFlying()
{
	return hitted && transform->position.y > 0;
}

bool Folie::Ball::started()
{
	return _started;
}

void Folie::Ball::setHitting(bool hitting_)
{
	hitting = hitting_;
}

void Folie::Ball::ballOnTheFloor()
{
	if (lastPlayerTouch != nullptr)
	{
		inHand = nullptr;
		hitted = false;
		hitting = false;
		ground = true;

		REF::game->ballOnGround(getActualField(), lastPlayerTouch);

		lastPlayerTouch = nullptr;
	}
}

void Folie::Ball::hit(Player ^playerTouch, Enums::eField field, UnityEngine::Vector2 coordinate2D, float angle_Deg)
{
	addForce(playerTouch, field, coordinate2D, angle_Deg);
}

void Folie::Ball::hit(Player ^playerTouch, Enums::eField field, Enums::eArea area, float angle_Deg)
{
	auto coordinate = GB::getCoordinates2DFromArea(field, area);
	hit(playerTouch, field, coordinate, angle_Deg);
}

void Folie::Ball::hit(Player ^playerTouch, Enums::eField field, Enums::ePosition position, float angle_Deg)
{
	auto coordinate = GB::getCoordinates2DFromPosition(field, position);
	hit(playerTouch, field, coordinate, angle_Deg);
}

void Folie::Ball::serve(Player ^playerTouch, Enums::eField field, UnityEngine::Vector2 coordinate2D)
{
	addForce(playerTouch, field, coordinate2D, Enums::serve_angle);
}

Folie::Enums::eField Folie::Ball::getActualField()
{
	return GB::getCampoFromCoordinates(transform->position.x, transform->position.z);
}

void Folie::Ball::addForce(Player ^playerTouch, Enums::eField field, UnityEngine::Vector2 coordinate2D, float angle_Deg)
{
	if (!ground && !hitting && ((inHand != nullptr) || (inHand == nullptr && lastPlayerTouch != playerTouch)))
	{
		hitting = true;
		inHand = nullptr;

		playerTouch->team->incTouch();

		lastPlayerTouch = playerTouch;
		REF::game->playerName->text = "Last touch: " + lastPlayerTouch->name;

		if (playerTouch->team->getTouch() > 3)
		{
			ground = true;
			REF::game->ballOnGround(getActualField(), lastPlayerTouch);
		}
		else
		{
			auto x_error = GB::rndInt16(-1, 1);
			auto z_error = GB::rndInt16(-1, 1);

			target2D = coordinate2D;

			destination2D = UnityEngine::Vector2(coordinate2D.x + x_error, coordinate2D.y + z_error);
			destination3D = UnityEngine::Vector3(destination2D.x, 0, destination2D.y);

			targetArea = GB::getAreaFromCoordinates(destination2D.x, destination2D.y);

			auto dir = UnityEngine::Vector3::operator-(destination3D, transform->position);

			auto height = dir.y;
			dir.y = 0;

			auto dist = dir.magnitude;

			auto angle_Rad = angle_Deg * UnityEngine::Mathf::Deg2Rad;

			dir.y = dist * UnityEngine::Mathf::Tan(angle_Rad);
			dist += height / UnityEngine::Mathf::Tan(angle_Rad);

			auto velocity = UnityEngine::Mathf::Sqrt(dist * UnityEngine::Physics::gravity.magnitude / UnityEngine::Mathf::Sin(2 * angle_Rad));

			rigidBody->velocity = velocity * dir.normalized;

			hitted = true;

			waiter->callAndWait(
				this,
				gcnew Action<bool>(this, &Ball::setHitting),
				gcnew array<bool ^> {false},
				REF::w4ms(800)
			);
		}
	}
}