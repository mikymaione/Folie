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

	create_BT_Rally();
	create_DT_hitTheBall();
}

// Unity
void Folie::Player::Start()
{
	phase = Enums::ePhase::null;

	startingArea = GB::getAreaFromPosition(startingPosition);
	currentArea = startingArea;
	currentPosition = startingPosition;

	rigidBody = GetComponent<UnityEngine::Rigidbody ^>();
	agent = GetComponent<UnityEngine::AI::NavMeshAgent ^>();
	hand = GB::GetComponentsInChildren<UnityEngine::Transform ^>(this, "Mano");

	_started = true;
}

// Unity
void Folie::Player::Update()
{
	BT_rally->Execute();
}


void Folie::Player::create_BT_Rally()
{
	auto q1 = gcnew AI::BTSelector();
	auto q3 = gcnew AI::BTSelector();
	auto q5 = gcnew AI::BTSelector();
	auto q6 = gcnew AI::BTSelector();

	auto s2 = gcnew AI::BTSequence();
	auto s4_2 = gcnew AI::BTSequence();
	auto s4_1 = gcnew AI::BTSequence();
	auto s6_1 = gcnew AI::BTSequence();
	auto s6_2 = gcnew AI::BTSequence();
	auto s6_3 = gcnew AI::BTSequence();
	auto s6_4 = gcnew AI::BTSequence();
	auto s6_5 = gcnew AI::BTSequence();

	BT_rally = gcnew AI::BT(q1);

	q1->AddChildren(s2);
	q1->AddChildren(gcnew AI::BTAction(gcnew Action(this, &Player::lookAtTarget)));

	s2->AddChildren(gcnew AI::BTQuestion(gcnew Func<bool>(this, &Player::Served)));
	s2->AddChildren(q3);

	q3->AddChildren(s4_1);
	q3->AddChildren(s4_2);

	s4_1->AddChildren(gcnew AI::BTQuestion(gcnew Func<bool>(this, &Player::ballIsFlying)));
	s4_1->AddChildren(gcnew AI::BTAction(gcnew Action<bool>(this, &Player::lookAtTheBall), true));
	s4_1->AddChildren(q5);

	s4_2->AddChildren(gcnew AI::BTAction(gcnew Action<bool>(this, &Player::lookAtTheBall), false));
	s4_2->AddChildren(gcnew AI::BTAction(gcnew Action(this, &Player::EnableAgent)));

	q5->AddChildren(s6_1);
	q5->AddChildren(s6_2);
	q5->AddChildren(s6_3);
	q5->AddChildren(s6_4);
	q5->AddChildren(s6_5);

	s6_1->AddChildren(gcnew AI::BTQuestion(gcnew Func<Enums::eGamePhase>(this, &Player::getGamePhase), Enums::eGamePhase::defence));
	s6_1->AddChildren(gcnew AI::BTAction(gcnew Action(this, &Player::playerTakePositionInReception)));

	s6_2->AddChildren(gcnew AI::BTQuestion(gcnew Func<bool>(this, &Player::ballIsReacheable)));
	s6_2->AddChildren(gcnew AI::BTAction(gcnew Action(this, &Player::hitTheBall)));

	s6_3->AddChildren(gcnew AI::BTQuestion(gcnew Func<bool>(this, &Player::canIReachTheBallJumping)));
	s6_3->AddChildren(gcnew AI::BTAction(gcnew Action<bool>(this, &Player::setJumping), true));

	s6_4->AddChildren(gcnew AI::BTQuestion(gcnew Func<bool>(this, &Player::isBallInMyArea)));
	s6_4->AddChildren(gcnew AI::BTAction(gcnew Action(this, &Player::takeCorrectPositionInAttackMode)));

	s6_5->AddChildren(gcnew AI::BTQuestion(gcnew Func<bool>(this, &Player::iAmJumping), false));
	s6_5->AddChildren(gcnew AI::BTAction(gcnew Action(this, &Player::takeCorrectPositionPreAttack)));
}

void Folie::Player::create_DT_hitTheBall()
{
	auto DT_hitTheBall = gcnew AI::DT<bool>(gcnew Func<bool>(this, &Player::ballIsReacheable));
	auto DT_getTouch = gcnew AI::DT<UInt16>(gcnew Func<UInt16>(this, &Player::getTouch));

	auto DT_getRole = gcnew AI::DT<Enums::eRole>(gcnew Func<Enums::eRole>(this, &Player::getRole));

	auto DT_pass = gcnew AI::DT<System::Object^>(gcnew Action(this, &Player::pass_));
	auto DT_set = gcnew AI::DT<System::Object^>(gcnew Action(this, &Player::set_));
	auto DT_attack = gcnew AI::DT<System::Object^>(gcnew Action(this, &Player::attack));

	auto DT_canIReachTheBallJumping = gcnew AI::DT<bool>(gcnew Func<bool>(this, &Player::canIReachTheBallJumping));
	auto DT_setJumping = gcnew AI::DT<System::Object^>(gcnew Action<bool>(this, &Player::setJumping), true);

	DT_hitTheBall->addDecision(true, DT_getTouch);
	DT_hitTheBall->addDecision(false, DT_canIReachTheBallJumping);

	DT_getTouch->addDecision(0, DT_pass);
	DT_getTouch->addDecision(1, DT_getRole);
	DT_getTouch->addDecision(2, DT_attack);

	DT_getRole->addDecision(Enums::eRole::Libero, DT_set);
	DT_getRole->addDecision(Enums::eRole::Setter, DT_set);

	DT_getRole->addDecision(Enums::eRole::OutsideHitter, DT_attack);
	DT_getRole->addDecision(Enums::eRole::MiddleBlocker, DT_attack);
	DT_getRole->addDecision(Enums::eRole::Opposite, DT_attack);

	DT_canIReachTheBallJumping->addDecision(true, DT_setJumping);
}

void Folie::Player::hitTheBall()
{
	DT_hitTheBall->Execute();
	team->playerThatSayMia = nullptr;
}

bool Folie::Player::isBallInMyArea()
{
	return (REF::ball->targetArea == currentArea);
}

bool Folie::Player::isBallInMyCourt()
{
	auto bC = GB::getCourtFromCoordinates(REF::ball->transform->position.z);
	auto mC = getCurrentCourt();

	return (bC == mC);
}

bool Folie::Player::canIReachTheBallJumping()
{
	auto attack_area = GB::getAttackArea(getCurrentCourt(), role, currentPosition, team->number_of_setters);
	auto coord_attack_area = GB::getCoordinates2DFromArea(field, attack_area);
	auto distanceFromBall = getDistanceFromBall();

	if (!jumping && GB::samePosition2D(REF::ball->target2D, coord_attack_area))
		if (team->getTouch() == 2)
			if (distanceFromBall < Enums::min_distance_to_jump && GB::canAttackJumping(role))
				return true;

	return false;
}

bool Folie::Player::ballIsReacheable()
{
	return getDistanceFromBall() < Enums::min_distance_to_hit && canTouchTheBall();
}

bool Folie::Player::Served()
{
	return (REF::ball != nullptr && phase != Enums::ePhase::serve);
}

void Folie::Player::lookAtTarget()
{
	if (lookingAt != nullptr)
		lookAt_(*lookingAt);
}

void Folie::Player::EnableAgent()
{
	agent->enabled = true;
}

void Folie::Player::takeCorrectPositionPreAttack()
{
	auto prendi_position = true;

	if (!team->serving)
		switch (role)
		{
		case Folie::Enums::eRole::Setter:
			if (team->number_of_setters > 1 && getCurrentCourt() == Enums::eCourt::back)
				prendi_position = (team->getTouch() > 0);
			break;
		case Folie::Enums::eRole::Opposite:
			prendi_position = (team->getTouch() > 0);
			break;
		case Folie::Enums::eRole::OutsideHitter:
		case Folie::Enums::eRole::MiddleBlocker:
			if (getCurrentCourt() == Enums::eCourt::back)
				prendi_position = (team->getTouch() > 0);

			break;
		}

	if (prendi_position)
	{
		auto attack_area = GB::getAttackArea(getCurrentCourt(), role, currentPosition, team->number_of_setters);
		auto coord_attack_area = GB::getCoordinates2DFromArea(field, attack_area);

		moveTo_Async(runSpeed, coord_attack_area.x, coord_attack_area.y);
	}
}

void Folie::Player::takeCorrectPositionInAttackMode()
{
	if (role == Enums::eRole::Setter && team->number_of_setters == 1 && team->getTouch() == 0)
	{
		auto front_middle_blocker = team->getPlayerWithRole(this, Enums::eRole::MiddleBlocker, Enums::eCourt::front);

		team->playerThatSayMia = front_middle_blocker;

		front_middle_blocker->moveTo_Async(runSpeed, REF::ball->destination2D.x, REF::ball->destination2D.y);
	}
	else
	{
		team->playerThatSayMia = this;
	}

	if (!jumping && GB::getCampoFromCoordinates(REF::ball->destination3D.x, REF::ball->destination3D.z) == field)
		if (canTouchTheBall())
		{
			moveTo_Async(runSpeed, REF::ball->destination3D.x, REF::ball->destination3D.z);
		}
		else
		{
			auto empty_pos = team->getAnEmptyPosition(currentPosition);
			auto empty_pos2D = GB::getCoordinates2DFromPosition(field, empty_pos);

			moveTo_Async(runSpeed, empty_pos2D.x, empty_pos2D.y);
		}
}

Folie::Enums::eGamePhase Folie::Player::getGamePhase()
{
	if (gamePhase != Enums::eGamePhase::serve || team->getTouch() > 0)
		gamePhase = (field == REF::ball->getActualField() ? Enums::eGamePhase::attack : Enums::eGamePhase::defence);

	return gamePhase;
}

Folie::Enums::eRole Folie::Player::getRole()
{
	return role;
}

UInt16 Folie::Player::getTouch()
{
	return team->getTouch();
}

float Folie::Player::getDistanceFromBall()
{
	auto d = UnityEngine::Vector3::Distance(REF::ball->transform->position, transform->position);

	return d;
}

void Folie::Player::playerTakePositionInReception()
{
	auto position = GB::getCoordinates2DFromPosition(field, currentPosition);

	if (gamePhase == Enums::eGamePhase::serve && team->number_of_setters == 1)
		switch (role)
		{
		case Folie::Enums::eRole::Setter:
			if (getCurrentCourt() == Enums::eCourt::back)
			{
				auto opposite = team->getPlayerWithRole(this, Enums::eRole::Opposite, Enums::eCourt::front);

				switch (currentPosition)
				{
				case Enums::ePosition::p5:
					position = GB::getCoordinates2DFromArea(field, Enums::eArea::a6S);
					break;
				case Enums::ePosition::p6:
					position = GB::getCoordinates2DFromArea(field, Enums::eArea::a1S);
					break;
				case Enums::ePosition::p1:
					position = GB::getCoordinates2DFromArea(field, Enums::eArea::a1E);
					break;
				}
			}
			break;
		case Folie::Enums::eRole::Opposite:
			if (getCurrentCourt() == Enums::eCourt::front)
			{
				auto setter = team->getPlayerWithRole(this, Enums::eRole::Setter, Enums::eCourt::back);
				position = GB::getCoordinates2DFromPosition(field, setter->currentPosition);
			}
			break;
		}

	playerTakePositionsOnTheField(position);
}

void Folie::Player::playerTakePositionsOnTheField(Enums::eField field_)
{
	field = field_;

	auto position = GB::getCoordinates2DFromPosition(field_, currentPosition);

	playerTakePositionsOnTheField(position);
}

void Folie::Player::playerTakePositionsOnTheField(UnityEngine::Vector2 position)
{
	agent->enabled = true;
	moveTo(walkSpeed, position.x, position.y);
	lookAtAnOpponent();
}

bool Folie::Player::started()
{
	return _started;
}

bool Folie::Player::inPosition()
{
	return GB::samePosition3D(agent->destination, transform->position);
}

void Folie::Player::moveTo_(float speed, float pos_x, float pos_z)
{
	auto d = UnityEngine::Vector3(pos_x, 0, pos_z);

	currentArea = GB::getAreaFromCoordinates(pos_x, pos_z);

	if (agent->enabled)
	{
		agent->speed = speed;
		agent->destination = d;
	}
}

void Folie::Player::moveTo(float speed, float pos_x, float pos_z)
{
	waiter->callAndWait(
		this,
		gcnew Action<float, float, float>(this, &Player::moveTo_),
		gcnew array<float ^> {speed, pos_x, pos_z},
		REF::wUntil(gcnew Func<bool>(this, &Player::inPosition))
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
	auto c = GB::getCoordinates2DFromArea(field, currentArea);
	moveTo(speed, c.x, c.y);
}

void Folie::Player::moveToPosition(float speed, Enums::ePosition position)
{
	currentArea = GB::getAreaFromPosition(position);
	move(speed);
}

void Folie::Player::moveToNextPosition()
{
	auto n = GB::getNextRotationPosition(currentPosition);
	auto d = GB::getCoordinates2DFromPosition(field, n);

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
		moveTo_(walkSpeed, dest.x, dest.z);

	return (dist < 1);
}

void Folie::Player::setJumping(bool j)
{
	agent->enabled = !j;
	jumping = j;

	if (j)
	{
		rigidBody->AddRelativeForce(UnityEngine::Vector3::up * rigidBody->mass * 3, UnityEngine::ForceMode::Impulse);

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

		lookAt(1, REF::ball->transform->position);
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
	auto dest_court = GB::oppositeField(field);

	auto y = target.y;

	switch (dest_court)
	{
	case Folie::Enums::eField::up:
		y = 2;
		break;
	case Folie::Enums::eField::down:
		y = 17;
		break;
	}

	REF::ball->serve(this, dest_court, UnityEngine::Vector2(target.x, y));
	REF::game->SM->run(REF::game->S_rally);
}

void Folie::Player::serve()
{
	targetChoosen = GB::selectRandomPosition(Enums::eCourt::back);
	auto c = GB::getCoordinates2DFromPosition(field, targetChoosen);

	auto seconds = GB::rndUInt16(1, 3);

	lookAt(seconds, c.x, c.y);

	waiter->callAndWait(
		this,
		gcnew Action<UnityEngine::Vector2>(this, &Player::serve_),
		gcnew array<UnityEngine::Vector2 ^> {c},
		REF::w4s(1)
	);

	moveToPosition(runSpeed, currentPosition);
}

void Folie::Player::takeTheBall()
{
	REF::ball->attachToHand(name);
	currentArea = Enums::eArea::a1S;
	move(walkSpeed);
}

bool Folie::Player::ballIsFlying()
{
	return REF::ball->ballIsFlying();
}

void Folie::Player::attack_(Enums::ePosition target)
{
	if (phase != Enums::ePhase::attack)
	{
		phase = Enums::ePhase::attack;
		REF::ball->hit(this, GB::oppositeField(field), target, calculateAngleOfAttack(target));
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

	if (jumping && court_mySelf == Enums::eCourt::back && court_target == Enums::eCourt::front)
		return Enums::serve_angle;
	else if (jumping && court_mySelf == Enums::eCourt::back && court_target == Enums::eCourt::back)
		return Enums::jump_attack_angle;
	else if (jumping && court_mySelf == Enums::eCourt::front && court_target == Enums::eCourt::back)
		return Enums::jump_kill_angle;
	else if (jumping && court_mySelf == Enums::eCourt::front && court_target == Enums::eCourt::front)
		return Enums::jump_attack_angle;
	else if (court_mySelf == Enums::eCourt::back && court_target == Enums::eCourt::back)
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

		team->playerThatSayMia = hitter;

		REF::ball->hit(this, field, area, Enums::pass_angle);
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
		REF::ball->hit(this, field, Enums::eArea::a2, Enums::pass_angle);
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
	auto milliseconds = GB::rndUInt16(50, 500);

	auto target = GB::selectRandomPosition();
	auto c = GB::getCoordinates2DFromPosition(GB::oppositeField(field), target);

	phase = Enums::ePhase::null;
	lookingAt = nullptr;

	auto to_ = UnityEngine::Vector3(c.x, transform->position.y, c.y);

	waiter->waitAndCall(
		this,
		REF::w4ms(milliseconds),
		gcnew Action<UnityEngine::Vector3>(this, &Player::lookAt_),
		gcnew array<UnityEngine::Vector3 ^> {to_}
	);
}

void Folie::Player::lookAt(UInt16 seconds, UnityEngine::Vector2 dest)
{
	lookAt(seconds, dest.x, dest.y);
}

void Folie::Player::lookAt(UInt16 seconds, float x, float z)
{
	auto to_ = UnityEngine::Vector3(x, 0, z);
	lookAt(seconds, to_);
}

void Folie::Player::lookAt(UInt16 seconds, UnityEngine::Vector3 to_)
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
	auto dest = UnityEngine::Vector3(to_.x, transform->position.y, to_.z);
	transform->LookAt(dest);
}

bool Folie::Player::iAmJumping()
{
	return jumping;
}

void Folie::Player::block()
{
	setJumping(true);
}

bool Folie::Player::canTouchTheBall()
{
	return (this != REF::ball->lastPlayerTouch);
}

Folie::Enums::eCourt Folie::Player::getCurrentCourt()
{
	auto c = GB::getCourtFromPosition(currentPosition);

	return c;
}

void Folie::Player::clearQueue()
{
	waiter->clearQueue();
}