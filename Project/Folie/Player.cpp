/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Player.h"
#include "REF.h"

void Folie::Player::Start()
{
	startingArea = GB::getAreaFromPosition(startingPosition);

	agent = GetComponent<UnityEngine::AI::NavMeshAgent ^>();
	mano = GB::GetComponentsInChildren<UnityEngine::Transform ^>(this, "Mano");
}

void Folie::Player::Update()
{

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
	REF::waiter->callAndWait(
		this,
		"moveTo_",
		gcnew array<float ^> {pos_x, pos_z},
		REF::wUntil(gcnew Func<bool>(this, &Player::inPosizione))
	);
}

void Folie::Player::moveTo_Async(float pos_x, float pos_z)
{
	moveTo_(pos_x, pos_z);
}

void Folie::Player::moveTo(UnityEngine::Vector3 ^position)
{
	moveTo(position->x, position->z);
}

void Folie::Player::move()
{
	auto c = GB::getCoordinatesFromArea(campo, currentArea);
	moveTo(c->x, c->y);
}

void Folie::Player::moveToPosition(Enums::ePosition position)
{
	currentPosition = position;
	currentArea = GB::getAreaFromPosition(position);

	move();
}

void Folie::Player::moveToNextPosition()
{
	moveToPosition(GB::getNextRotationPosition(currentPosition));
}

void Folie::Player::pass_mode()
{
	if (UnityEngine::Vector3::Distance(transform->position, REF::ball->transform->position) < 1)
		hit();
}

void Folie::Player::serveRitual()
{
	lookAt(1, REF::ball->transform->position);
	moveTo(REF::ball->transform->position);

	REF::waiter->callAndWait(
		this,
		"takeTheBall",
		REF::wUntil(gcnew Func<bool>(REF::ball, &Ball::ballInHand))
	);

	REF::waiter->callAndWait(
		this,
		"serve",
		REF::w4s(0.1)
	);
}

void Folie::Player::serve()
{
	targetChoosen = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(campo, targetChoosen);

	lookAt(2, c->x, c->y);
	hit(targetChoosen);

	moveToPosition(startingPosition);
}

void Folie::Player::takeTheBall()
{
	REF::ball->attachToHand(name);
	currentArea = Enums::eArea::a1S;
	move();
}

void Folie::Player::hit(Enums::ePosition target)
{
	//if (distance_from_ball < 0.5)		
	REF::waiter->callAndWait(
		this,
		"hit_",
		gcnew array<Enums::ePosition ^> {target},
		REF::w4s(0.1)
	);
}

void Folie::Player::hit_(Enums::ePosition target)
{
	REF::ball->moveTo(GB::oppositeField(campo), target);
}

void Folie::Player::hit()
{
	auto target = GB::selectRandomPosition();

	hit(target);
}

void Folie::Player::lookAtAnOpponent(float seconds)
{
	auto target = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(GB::oppositeField(campo), target);

	lookAt(seconds, c);
}

void Folie::Player::lookAt(float seconds, UnityEngine::Vector2 ^dest)
{
	lookAt(seconds, dest->x, dest->y);
}

void Folie::Player::lookAt(float seconds, float x, float y)
{
	auto to_ = UnityEngine::Vector3(x, transform->position.y, y);

	lookAt(seconds, to_);
}

void Folie::Player::lookAt(float seconds, UnityEngine::Vector3 to_)
{
	lookingAt = to_;

	REF::waiter->callAndWait(
		this,
		"lookAt_",
		gcnew array<UnityEngine::Vector3 ^> {lookingAt},
		REF::w4s(seconds)
	);
}

void Folie::Player::lookAt_(UnityEngine::Vector3 to_)
{
	transform->LookAt(to_);
}

/*
void Folie::Player::propagateEvent(GB::eEvent e)
{
	switch (e)
	{
	case GB::eEvent::giocatoriPrenderePosizioniInCampo:
		moveToPosition(GB::eEvent::giocatoriPrenderePosizioniInCampo_end, startingPosition);
		break;

	case GB::eEvent::giocatoriPrenderePosizioniInCampo_end:
		event_bubbleUp(e);
		break;

	case GB::eEvent::lookAtOpponent:
		lookAtAnOpponent();
		break;

	case GB::eEvent::lookAtTheBall:
		lookingAtTheBall = true;
		break;

	case GB::eEvent::lookAtTheBall_end:
		lookingAtTheBall = false;
		break;

	case GB::eEvent::takeTheBall:
		if (currentPosition == Enums::ePosition::p1)
			moveTo(GB::eEvent::takeTheBall_end, REF::ball->pos_x, REF::ball->pos_z);
		break;

	case GB::eEvent::takeTheBall_end:
		REF::ball->attachToHand(name);
		currentArea = Enums::eArea::a1S;
		move(GB::eEvent::gotoServingPosition_end);
		break;

	case GB::eEvent::gotoServingPosition_end:
		serve();
		event_bubbleUp(GB::eEvent::gotoServingPosition_end);
		break;

	case GB::eEvent::serve:
		hit(targetChoosen);
		event_bubbleUp(GB::eEvent::serve_do);
		moveToPosition(GB::eEvent::serve_end, currentPosition);
		break;

	}
}
*/