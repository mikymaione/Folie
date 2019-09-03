/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Player.h"
#include "REF.h"

Folie::Player::Player()
{
	waiter = gcnew CoroutineQueue();
}

void Folie::Player::Start()
{
	phase = Enums::ePhase::null;

	startingArea = GB::getAreaFromPosition(startingPosition);
	currentArea = startingArea;
	currentPosition = startingPosition;

	agent = GetComponent<UnityEngine::AI::NavMeshAgent ^>();
	mano = GB::GetComponentsInChildren<UnityEngine::Transform ^>(this, "Mano");
}

void Folie::Player::Update()
{
	if (REF::ball != nullptr)
	{
		auto ballIsFlying = REF::ball->ballIsFlying();
		auto distanceFromBall = getDistanceFromBall();

		lookAtTheBall(ballIsFlying);

		if (ballIsFlying)
		{
			if (distanceFromBall < Enums::min_distance_to_hit)
			{
				switch (REF::ball->touch)
				{
				case 0:
					pass_();
					break;
				case 1:
					set_();
					break;
				case 2:
					attack_(GB::selectRandomPosition());
					break;
				}

				team->playerThatSayMia = nullptr;
				REF::game->mine->text = "Mine:";
			}
			else if (distanceFromBall < Enums::min_distance_to_move_to_the_ball && team->playerThatSayMia == nullptr)
			{
				team->playerThatSayMia = this;
				REF::game->mine->text = "Mine: " + name;
				agent->destination = REF::ball->destination;
			}
			else
			{
				auto myPos = GB::getCoordinatesFromPosition(campo, currentPosition);
				moveTo_Async(myPos.x, myPos.y);
			}
		}
	}

	if (lookingAt != nullptr)
		lookAt_(*lookingAt);
}

float Folie::Player::getDistanceFromBall()
{
	auto d = UnityEngine::Vector3::Distance(REF::ball->transform->position, transform->position);

	return d;
}

bool Folie::Player::inPosizione()
{
	return GB::samePosition(agent->destination, transform->position);
}

void Folie::Player::moveTo_(float pos_x, float pos_z)
{
	auto d = UnityEngine::Vector3(pos_x, 0, pos_z);
	currentArea = GB::getAreaFromCoordinates(pos_x, pos_z);
	agent->destination = d;
}

void Folie::Player::moveTo(float pos_x, float pos_z)
{
	waiter->callAndWait(
		this,
		gcnew Action<float, float>(this, &Player::moveTo_),
		gcnew array<float ^> {pos_x, pos_z},
		REF::wUntil(gcnew Func<bool>(this, &Player::inPosizione))
	);
}

void Folie::Player::moveTo_Async(float pos_x, float pos_z)
{
	moveTo_(pos_x, pos_z);
}

void Folie::Player::moveTo(UnityEngine::Vector3 position)
{
	moveTo(position.x, position.z);
}

void Folie::Player::move()
{
	auto c = GB::getCoordinatesFromArea(campo, currentArea);
	moveTo(c.x, c.y);
}

void Folie::Player::moveToPosition(Enums::ePosition position)
{
	currentArea = GB::getAreaFromPosition(position);
	move();
}

void Folie::Player::moveToNextPosition()
{
	auto n = GB::getNextRotationPosition(currentPosition);
	auto d = GB::getCoordinatesFromPosition(campo, n);

	currentPosition = n;

	moveTo_Async(d.x, d.y);
}

void Folie::Player::moveTo(UnityEngine::Component ^comp)
{
	goToElement = comp;

	waiter->callAndWait(
		this,
		gcnew Action<float, float>(this, &Player::moveTo_),
		gcnew array<float ^> {comp->transform->position.x, comp->transform->position.z},
		REF::wUntil(gcnew Func<bool>(this, &Player::goToElementReached))
	);
}

bool Folie::Player::goToElementReached()
{
	auto dest = goToElement->transform->position;
	auto dist = UnityEngine::Vector3::Distance(transform->position, dest);

	if (dist > 1)
		moveTo_(dest.x, dest.z);

	return (dist < 1);
}

void Folie::Player::serveRitual()
{
	if (phase != Enums::ePhase::serve)
	{
		phase = Enums::ePhase::serve;

		lookAt(0.3, REF::ball->transform->position);
		moveTo(REF::ball);

		waiter->callAndWait(
			this,
			gcnew Action(this, &Player::takeTheBall),
			REF::wUntil(gcnew Func<bool>(REF::ball, &Ball::ballInHand))
		);

		waiter->callAndWait(
			this,
			gcnew Action(this, &Player::serve),
			REF::w4ms(100)
		);
	}
}

void Folie::Player::serve_(UnityEngine::Vector2 target)
{
	if (getDistanceFromBall() < Enums::min_distance_to_hit)
	{
		auto dest_court = GB::oppositeField(campo);

		auto y = target.y;

		switch (dest_court)
		{
		case Folie::Enums::eCampo::up:
			y = 1;
			break;
		case Folie::Enums::eCampo::down:
			y = 18;
			break;
		}

		REF::ball->serve(this, dest_court, UnityEngine::Vector2(target.x, y));
	}
}

void Folie::Player::serve()
{
	targetChoosen = GB::selectRandomPosition(Enums::eCourt::back);
	auto c = GB::getCoordinatesFromPosition(campo, targetChoosen);

	lookAt(2, c.x, c.y);

	waiter->callAndWait(
		this,
		gcnew Action<UnityEngine::Vector2>(this, &Player::serve_),
		gcnew array<UnityEngine::Vector2 ^> {c},
		REF::w4s(1)
	);

	moveToPosition(currentPosition);
}

void Folie::Player::takeTheBall()
{
	REF::ball->attachToHand(name);
	currentArea = Enums::eArea::a1S;
	move();
}

void Folie::Player::attack_(Enums::ePosition target)
{
	if (phase != Enums::ePhase::attack)
	{
		phase = Enums::ePhase::attack;
		REF::ball->hit(this, GB::oppositeField(campo), target, calculateAngleOfAttack(target));
	}
}

void Folie::Player::attack(Enums::ePosition target)
{
	waiter->callAndWait(
		this,
		gcnew Action<Enums::ePosition>(this, &Player::attack_),
		gcnew array<Enums::ePosition ^> {target},
		REF::w4s(1)
	);
}

void Folie::Player::attack()
{
	auto target = GB::selectRandomPosition();

	attack(target);
}

float Folie::Player::calculateAngleOfAttack(Enums::ePosition target)
{
	auto court_target = GB::getCourtFromPosition(target);
	auto court_mySelf = this->getCurrentCourt();

	if (court_mySelf == Enums::eCourt::back && court_target == Enums::eCourt::back)
		return Enums::serve_angle;
	else if (court_mySelf == Enums::eCourt::back && court_target == Enums::eCourt::front)
		return Enums::attack_angle;
	else
		return Enums::pass_angle;
}

void Folie::Player::set()
{
	waiter->callAndWait(
		this,
		gcnew Action(this, &Player::set_),
		REF::w4s(1)
	);
}

void Folie::Player::set_()
{
	if (phase != Enums::ePhase::set)
	{
		phase = Enums::ePhase::set;

		auto randomRole = Enums::eRole::Opposite;
		auto randomCourt = Enums::eCourt::front;

		auto r = GB::rndUInt16(1, 30);
		if (r % 2 == 0)
			randomRole = Enums::eRole::OutsideHitter;
		else if (r % 3 == 0)
			randomRole = Enums::eRole::MiddleBlocker;

		auto c = GB::rndUInt16(1, 3);
		if (c % 3 == 0)
			randomCourt = Enums::eCourt::back;

		auto hitter = team->getPlayerWithRole(this, randomRole, randomCourt);

		REF::ball->hit(this, campo, hitter->currentArea, Enums::pass_angle);
	}
}

void Folie::Player::pass()
{
	waiter->callAndWait(
		this,
		gcnew Action(this, &Player::pass_),
		REF::w4s(1)
	);
}

void Folie::Player::pass_()
{
	if (phase != Enums::ePhase::serve && phase != Enums::ePhase::pass)
	{
		phase = Enums::ePhase::pass;

		auto setter = team->getPlayerWithRole(this, Enums::eRole::Setter, Enums::eCourt::front);
		REF::ball->hit(this, campo, setter->currentArea, Enums::pass_angle);
	}
}

void Folie::Player::lookAtTheBall(bool looking)
{
	if (looking)
		lookingAt = REF::ball->transform->position;
	else
		lookingAt = nullptr;
}

void Folie::Player::lookAtAnOpponent()
{
	float t = GB::rndUInt16(1, 20);
	float seconds = t / 10.0f;

	auto target = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(GB::oppositeField(campo), target);

	phase = Enums::ePhase::null;
	lookingAt = nullptr;

	auto to_ = UnityEngine::Vector3(c.x, transform->position.y, c.y);

	waiter->waitAndCall(
		this,
		REF::w4s(seconds),
		gcnew Action<UnityEngine::Vector3>(this, &Player::lookAt_),
		gcnew array<UnityEngine::Vector3 ^> {to_}
	);
}

void Folie::Player::lookAt(float seconds, UnityEngine::Vector2 dest)
{
	lookAt(seconds, dest.x, dest.y);
}

void Folie::Player::lookAt(float seconds, float x, float z)
{
	auto to_ = UnityEngine::Vector3(x, 0, z);
	lookAt(seconds, to_);
}

void Folie::Player::lookAt(float seconds, UnityEngine::Vector3 to_)
{
	waiter->callAndWait(
		this,
		gcnew Action<UnityEngine::Vector3>(this, &Player::lookAt_),
		gcnew array<UnityEngine::Vector3 ^> {to_},
		REF::w4s(seconds)
	);
}

void Folie::Player::lookAt_(UnityEngine::Vector3 to_)
{
	auto to_y = UnityEngine::Vector3(to_.x, transform->position.y, to_.z);
	transform->LookAt(to_y);
}

Folie::Enums::eCourt Folie::Player::getCurrentCourt()
{
	auto c = GB::getCourtFromCoordinates(transform->position.z);

	return c;
}