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

void Folie::Player::moveTo_()
{
	currentArea = GB::getAreaFromCoordinates(destination.x, destination.z);
	agent->destination = destination;
}

void Folie::Player::moveTo(Enums::eJob job, float pos_x, float pos_z)
{
	destination = UnityEngine::Vector3(pos_x, 0, pos_z);

	REF::coRunner->Enqueue(gcnew CoRunnerItem(
		job,
		Enums::eMode::First,
		gcnew Action(this, &Player::moveTo_),
		REF::wUntil(gcnew Func<bool>(this, &Player::inPosizione))
	));
}

void Folie::Player::moveTo(float pos_x, float pos_z)
{
	moveTo(Enums::eJob::Sync, pos_x, pos_z);
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

void Folie::Player::serve()
{
	moveTo(REF::ball->transform->position);

	targetChoosen = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(campo, targetChoosen);

	lookAt(c->x, c->y);
	hit(targetChoosen);

	moveToPosition(startingPosition);
}

void Folie::Player::hit(Enums::ePosition target)
{
	//if (distance_from_ball < 0.5)	
	hitTarget = target;

	REF::coRunner->Enqueue(gcnew CoRunnerItem(
		Enums::eJob::Sync,
		Enums::eMode::First,
		gcnew Action(this, &Player::hit_),
		REF::w4ms(0.1)
	));
}

void Folie::Player::hit_()
{
	REF::ball->moveTo(GB::oppositeField(campo), hitTarget);
}

void Folie::Player::hit()
{
	auto target = GB::selectRandomPosition();

	hit(target);
}

void Folie::Player::lookAtAnOpponent()
{
	auto target = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(GB::oppositeField(campo), target);

	lookAt(c);
}

void Folie::Player::lookAt(UnityEngine::Vector2 ^dest)
{
	lookAt(dest->x, dest->y);
}

void Folie::Player::lookAt(float x, float y)
{
	lookingAt = UnityEngine::Vector3(x, transform->position.y, y);

	lookAt();
}

void Folie::Player::lookAt()
{
	transform->LookAt(lookingAt);
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