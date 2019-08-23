/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Player.h"
#include "REF.h"

Folie::Player::Player(String ^name_, GB::eCampo campo_, GB::ePosition startingPosition_, GB::eRole role_)
{
	name = name_;
	role = role_;
	campo = campo_;
	startingPosition = startingPosition_;
}

void Folie::Player::moveTo(GB::eEvent e, float pos_x, float pos_z)
{
	event_moveAt(e, pos_x, pos_z);
}

void Folie::Player::move(GB::eEvent e)
{
	auto xy = GB::getCoordinatesFromArea(campo, currentArea);
	moveTo(e, xy.X, xy.Y);
}

void Folie::Player::moveToPosition(GB::eEvent e, GB::ePosition position)
{
	currentPosition = position;
	currentArea = GB::getAreaFromPosition(position);

	move(e);
}

void Folie::Player::moveToNextPosition(GB::eEvent e)
{
	moveToPosition(e, GB::getNextRotationPosition(currentPosition));
}

void Folie::Player::pass(Ball ^ball)
{
	auto distanceToTheBall = GB::distanceBetweenTwoPoints3D(ball->pos_x, ball->pos_y, ball->pos_z, pos_x, pos_y, pos_z);

	if (distanceToTheBall < 1)
		ball->moveTo(campo, GB::ePosition::p3);
}

void Folie::Player::serve(Ball ^ ball)
{
	targetChoosen = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(campo, targetChoosen);

	event_LookAt(GB::eEvent::serve, c.X, c.Y);
}

void Folie::Player::hit(Ball ^ball, GB::ePosition target)
{
	auto distanceToTheBall = GB::distanceBetweenTwoPoints3D(ball->pos_x, ball->pos_y, ball->pos_z, pos_x, pos_y, pos_z);

	//if (distanceToTheBall < 0.5)	
	ball->moveTo(GB::oppositeField(campo), target);
}

void Folie::Player::hit(Ball ^ball)
{
	auto target = GB::selectRandomPosition();

	hit(ball, target);
}

void Folie::Player::lookAtAnOpponent()
{
	auto target = GB::selectRandomPosition();
	auto p = GB::getCoordinatesFromPosition(GB::oppositeField(campo), target);

	event_LookAt(GB::eEvent::lookAtOpponent_end, p.X, p.Y);
}

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
		if (currentPosition == GB::ePosition::p1)
			moveTo(GB::eEvent::takeTheBall_end, REF::ball->pos_x, REF::ball->pos_z);
		break;

	case GB::eEvent::takeTheBall_end:
		REF::ball->attachToHand(name);
		currentArea = GB::eArea::a1S;
		move(GB::eEvent::gotoServingPosition_end);
		break;

	case GB::eEvent::gotoServingPosition_end:
		serve(REF::ball);
		event_bubbleUp(GB::eEvent::gotoServingPosition_end);
		break;

	case GB::eEvent::serve:
		hit(REF::ball, targetChoosen);
		moveToPosition(GB::eEvent::serve_end, currentPosition);
		break;

	}
}