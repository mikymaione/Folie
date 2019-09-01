/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Ball.h"
#include "REF.h"

Folie::Ball::Ball()
{
	waiter = gcnew Waiter();
}

void Folie::Ball::Start()
{
	rigidBody = GetComponent<UnityEngine::Rigidbody^>();
}

void Folie::Ball::Update()
{
	if (campoPrecedente != getCampoAttuale())
		touch = 0;

	campoPrecedente = getCampoAttuale();

	if (inMano != nullptr)
		transform->SetPositionAndRotation(inMano->position, inMano->rotation);

	if (transform->position.y < -0.3)
		transform->position = UnityEngine::Vector3(11.69f, 0.4f, 7.93f);

	REF::game->touch->text = "Touch: " + touch;
}

void Folie::Ball::OnCollisionEnter(UnityEngine::Collision collision)
{
	auto gobj = collision.collider->gameObject;

	if (gobj->CompareTag("Player"))
	{
		lastPlayerTouch = gobj->GetComponent<Player ^>();
	}
	else if (hitted && gobj->CompareTag("Pavimento"))
	{
		ground = true;
		hitted = false;
		touch = 0;

		REF::game->ballOnGround(getCampoAttuale(), lastPlayerTouch);
	}
}

void Folie::Ball::attachToHand(String ^player_name)
{
	for each (auto team in REF::teams)
		for each (auto p in team->players)
			if (p->name->Equals(player_name))
			{
				inMano = p->mano;
				REF::game->playerName->text = "Player: " + p->name;

				hitted = false;
				ground = false;

				break;
			}
}

bool Folie::Ball::ballInHand()
{
	return inMano != nullptr;
}

bool Folie::Ball::ballIsFlying()
{
	return hitted && transform->position.y > 0;
}

void Folie::Ball::setHitting(bool hitting_)
{
	hitting = hitting_;
}

void Folie::Ball::hit(Player ^playerTouch, Enums::eCampo campo, UnityEngine::Vector2 coordinate, float angle)
{
	if (!hitting)
	{
		hitting = true;
		touch++;

		addForce(playerTouch, campo, coordinate, angle);
	}
}

void Folie::Ball::hit(Player ^playerTouch, Enums::eCampo campo, Enums::eArea area, float angle)
{
	if (!hitting)
	{
		auto coordinate = GB::getCoordinatesFromArea(campo, area);
		hit(playerTouch, campo, coordinate, angle);
	}
}

void Folie::Ball::hit(Player ^playerTouch, Enums::eCampo campo, Enums::ePosition position, float angle)
{
	if (!hitting)
	{
		auto coordinate = GB::getCoordinatesFromPosition(campo, position);
		hit(playerTouch, campo, coordinate, angle);
	}
}

void Folie::Ball::serve(Player ^playerTouch, Enums::eCampo campo, UnityEngine::Vector2 coordinate)
{
	if (!hitting)
	{
		touch = 0;
		addForce(playerTouch, campo, coordinate, Enums::serve_angle);
	}
}

Folie::Enums::eCampo Folie::Ball::getCampoAttuale()
{
	auto c = GB::getCampoFromCoordinates(transform->position.x, transform->position.z);

	return c;
}

void Folie::Ball::addForce(Player ^playerTouch, Enums::eCampo campo, UnityEngine::Vector2 coordinate, float angle)
{
	if (!ground)
	{
		inMano = nullptr;

		lastPlayerTouch = playerTouch;
		REF::game->playerName->text = "Player: " + lastPlayerTouch->name;

		if (touch > 3)
		{
			ground = true;
			REF::game->ballOnGround(getCampoAttuale(), lastPlayerTouch);
		}
		else
		{
			auto a = angle * UnityEngine::Mathf::Deg2Rad;

			auto destination = UnityEngine::Vector3(coordinate.x, 0, coordinate.y);

			auto dir = UnityEngine::Vector3::operator-(destination, transform->position);

			auto height = dir.y;
			dir.y = 0;

			auto dist = dir.magnitude;

			dir.y = dist * UnityEngine::Mathf::Tan(a);
			dist += height / UnityEngine::Mathf::Tan(a);

			auto velocity = UnityEngine::Mathf::Sqrt(dist * UnityEngine::Physics::gravity.magnitude / UnityEngine::Mathf::Sin(2 * a));

			rigidBody->velocity = velocity * dir.normalized;

			hitted = true;

			waiter->callAndWait(
				this,
				gcnew Action<bool>(this, &Ball::setHitting),
				gcnew array<bool ^> {false},
				REF::w4ms(150)
			);
		}
	}
}