/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Player.hpp"
#include "REF.hpp"

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

	rigidBody = GetComponent<UnityEngine::Rigidbody ^>();
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
			auto attack_area = GB::getAttackArea(getCurrentCourt(), role, currentPosition, team->number_of_setters);
			auto coord_attack_area = GB::getCoordinates2DFromArea(campo, attack_area);

			if (distanceFromBall < Enums::min_distance_to_hit && campo == REF::ball->getCampoAttuale())
			{
				switch (team->touch)
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
				team->setMine("");
			}
			else if (GB::samePosition2D(REF::ball->target2D, GB::getCoordinates2DFromArea(campo, currentArea)))
			{
				if (role == Enums::eRole::Setter && team->touch == 0)
				{
					auto front_middle_blocker = team->getPlayerWithRole(this, Enums::eRole::MiddleBlocker, Enums::eCourt::front);

					team->playerThatSayMia = front_middle_blocker;

					front_middle_blocker->moveTo_Async(runSpeed, REF::ball->destination2D.x, REF::ball->destination2D.y);
				}
				else
				{
					team->playerThatSayMia = this;
				}

				team->setMine(name);

				if (!jumping)
					agent->destination = REF::ball->destination3D;
			}
			else if (!jumping && GB::samePosition2D(REF::ball->target2D, coord_attack_area))
			{
				switch (team->touch)
				{
				case 2:
					if (distanceFromBall < Enums::min_distance_to_jump && GB::canAttackJumping(role))
						setJumping(true);
					break;
				}
			}
			else if (!jumping)
			{
				auto prendi_posizione = true;

				if (!team->serving)
					switch (role)
					{
					case Folie::Enums::eRole::Opposite:
						prendi_posizione = (team->touch > 0);
						break;
					case Folie::Enums::eRole::OutsideHitter:
					case Folie::Enums::eRole::MiddleBlocker:
						if (getCurrentCourt() == Enums::eCourt::back)
							prendi_posizione = (team->touch > 0);

						break;
					}

				if (prendi_posizione)
					moveTo_Async(runSpeed, coord_attack_area.x, coord_attack_area.y);
			}

			switch (campo)
			{
			case Folie::Enums::eCampo::up:
				if (transform->position.z > 9.8f)
					transform->position.Set(transform->position.x, transform->position.y, 9.9f);
				break;
			case Folie::Enums::eCampo::down:
				if (transform->position.z < 10.2f)
					transform->position.Set(transform->position.x, transform->position.y, 10.2f);
				break;
			}
		}
		else
		{
			agent->enabled = true;
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

void Folie::Player::giocatorePrenderePosizioniInRicezione()
{
	auto posizione = GB::getCoordinates2DFromPosition(campo, currentPosition);

	if (team->number_of_setters == 1)
		switch (role)
		{
		case Folie::Enums::eRole::Setter:
			if (getCurrentCourt() == Enums::eCourt::back)
			{
				auto opposite = team->getPlayerWithRole(this, Enums::eRole::Opposite, Enums::eCourt::front);

				switch (currentPosition)
				{
				case Enums::ePosition::p5:
					posizione = GB::getCoordinates2DFromArea(campo, Enums::eArea::a6S);
					break;
				case Enums::ePosition::p6:
					posizione = GB::getCoordinates2DFromArea(campo, Enums::eArea::a1S);
					break;
				case Enums::ePosition::p1:
					posizione = GB::getCoordinates2DFromArea(campo, Enums::eArea::a1E);
					break;
				}
			}
			break;
		case Folie::Enums::eRole::Opposite:
			if (getCurrentCourt() == Enums::eCourt::front)
			{
				auto setter = team->getPlayerWithRole(this, Enums::eRole::Setter, Enums::eCourt::back);
				posizione = GB::getCoordinates2DFromPosition(campo, setter->currentPosition);
			}
			break;
		}

	giocatorePrenderePosizioniInCampo(posizione);
}

void Folie::Player::giocatorePrenderePosizioniInCampo(Enums::eCampo campo_)
{
	campo = campo_;

	auto posizione = GB::getCoordinates2DFromPosition(campo_, currentPosition);

	giocatorePrenderePosizioniInCampo(posizione);
}

void Folie::Player::giocatorePrenderePosizioniInCampo(UnityEngine::Vector2 posizione)
{
	agent->enabled = true;
	moveTo_Async(walkSpeed, posizione.x, posizione.y);
}

bool Folie::Player::inPassPosition()
{
	return GB::samePosition3D(agent->destination, transform->position);
}

void Folie::Player::moveTo_(float speed, float pos_x, float pos_z)
{
	auto d = UnityEngine::Vector3(pos_x, 0, pos_z);

	currentArea = GB::getAreaFromCoordinates(pos_x, pos_z);

	agent->speed = speed;
	agent->destination = d;
}

void Folie::Player::moveTo(float speed, float pos_x, float pos_z)
{
	waiter->callAndWait(
		this,
		gcnew Action<float, float, float>(this, &Player::moveTo_),
		gcnew array<float ^> {speed, pos_x, pos_z},
		REF::wUntil(gcnew Func<bool>(this, &Player::inPassPosition))
	);
}

void Folie::Player::moveTo_Async(float speed, float pos_x, float pos_z)
{
	moveTo_(speed, pos_x, pos_z);
}

void Folie::Player::moveTo(float speed, UnityEngine::Vector3 position)
{
	moveTo(speed, position.x, position.z);
}

void Folie::Player::move(float speed)
{
	auto c = GB::getCoordinates2DFromArea(campo, currentArea);
	moveTo(speed, c.x, c.y);
}

void Folie::Player::moveToPosition(Enums::ePosition position)
{
	currentArea = GB::getAreaFromPosition(position);
	move(walkSpeed);
}

void Folie::Player::moveToNextPosition()
{
	auto n = GB::getNextRotationPosition(currentPosition);
	auto d = GB::getCoordinates2DFromPosition(campo, n);

	currentPosition = n;

	moveTo_Async(walkSpeed, d.x, d.y);
}

void Folie::Player::moveTo(float speed, UnityEngine::Component ^comp)
{
	goToElement = comp;

	waiter->callAndWait(
		this,
		gcnew Action<float, float, float>(this, &Player::moveTo_),
		gcnew array<float ^> {speed, comp->transform->position.x, comp->transform->position.z},
		REF::wUntil(gcnew Func<bool>(this, &Player::goToElementReached))
	);
}

bool Folie::Player::goToElementReached()
{
	auto dest = goToElement->transform->position;
	auto dist = UnityEngine::Vector3::Distance(transform->position, dest);

	if (dist > 1)
		moveTo_(runSpeed, dest.x, dest.z);

	return (dist < 1);
}

void Folie::Player::setJumping(bool j)
{
	agent->enabled = !j;
	jumping = j;

	if (j)
	{
		auto pos_of_landing2D = GB::getCoordinates2DFromPosition(campo, currentPosition);
		auto pos_of_landing3D = UnityEngine::Vector3(pos_of_landing2D.x, 0, pos_of_landing2D.y);
		auto myPos = UnityEngine::Vector3(transform->position.x, 0, transform->position.z);
		auto dir = UnityEngine::Vector3::operator-(myPos, pos_of_landing3D);
		dir = UnityEngine::Vector3::operator+(UnityEngine::Vector3::up, dir.normalized);

		rigidBody->AddRelativeForce(dir * rigidBody->mass * 3, UnityEngine::ForceMode::Impulse);

		waiter->callAndWait(
			this,
			gcnew Action<bool>(this, &Player::setJumping),
			gcnew array<bool ^> {false},
			REF::w4s(1)
		);
	}
}

void Folie::Player::serveRitual()
{
	if (phase != Enums::ePhase::serve)
	{
		phase = Enums::ePhase::serve;

		lookAt(0.3, REF::ball->transform->position);
		moveTo(walkSpeed, REF::ball);

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
			y = 2;
			break;
		case Folie::Enums::eCampo::down:
			y = 17;
			break;
		}

		REF::ball->serve(this, dest_court, UnityEngine::Vector2(target.x, y));
	}
}

void Folie::Player::serve()
{
	targetChoosen = GB::selectRandomPosition(Enums::eCourt::back);
	auto c = GB::getCoordinates2DFromPosition(campo, targetChoosen);

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
	move(walkSpeed);
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
		auto area = GB::getAttackArea(hitter->getCurrentCourt(), hitter->role, hitter->currentPosition, team->number_of_setters);

		REF::ball->hit(this, campo, area, Enums::pass_angle);
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

		//auto setter = team->getPlayerWithRole(this, Enums::eRole::Setter, Enums::eCourt::front);
		REF::ball->hit(this, campo, Enums::eArea::a2, Enums::pass_angle);
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
	auto c = GB::getCoordinates2DFromPosition(GB::oppositeField(campo), target);

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
	auto c = GB::getCourtFromPosition(currentPosition);

	return c;
}