/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Player.h"

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
	auto target = GB::selectRandomPosition();
	auto c = GB::getCoordinatesFromPosition(campo, target);

	event_LookAt(GB::eEvent::serve, c.X, c.Y);
}

void Folie::Player::hit(Ball ^ball, GB::ePosition target)
{
	auto distanceToTheBall = GB::distanceBetweenTwoPoints3D(ball->pos_x, ball->pos_y, ball->pos_z, pos_x, pos_y, pos_z);

	//if (distanceToTheBall < 0.5)
	ball->moveTo(campo, target);
}

void Folie::Player::hit(Ball ^ball)
{
	auto target = GB::selectRandomPosition();

	hit(ball, target);
}

void Folie::Player::propagateEvent(GB::eEvent e)
{
	auto _ball = (Ball ^)GB::ball;

	switch (e)
	{
	case Folie::GB::eEvent::giocatoriPrenderePosizioniInCampo:
		moveToPosition(GB::eEvent::giocatoriPrenderePosizioniInCampo_end, startingPosition);
		break;

	case Folie::GB::eEvent::giocatoriPrenderePosizioniInCampo_end:
		event_bubbleUp(e);
		break;

	case Folie::GB::eEvent::lookAtTheBall:
		event_LookAt(GB::eEvent::lookAtTheBall_end, _ball->pos_x, _ball->pos_z);
		break;

	case Folie::GB::eEvent::takeTheBall:
		if (currentPosition == GB::ePosition::p1)
			moveTo(GB::eEvent::takeTheBall_end, _ball->pos_x, _ball->pos_z);
		break;

	case Folie::GB::eEvent::takeTheBall_end:
		_ball->attachToHand(name);
		currentArea = GB::eArea::a1S;
		move(GB::eEvent::gotoServingPosition_end);
		break;

	case Folie::GB::eEvent::gotoServingPosition_end:
		serve(_ball);
		break;

	case Folie::GB::eEvent::serve:
		hit(_ball);
		moveToPosition(GB::eEvent::serve_end, currentPosition);
		break;

	}
}