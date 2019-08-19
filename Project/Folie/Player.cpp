/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Player.h"

Folie::Player::Player(String ^name_, GB::ePosition startingPosition_)
{
	name = name_;

	startingPosition = startingPosition_;
	currentPosition = startingPosition_;
	currentArea = Folie::GB::getAreaFromPosition(startingPosition_);
}

void Folie::Player::moveToNextPosition()
{
	currentPosition = GB::getNextRotationPosition(currentPosition);
	currentArea = GB::getAreaFromPosition(currentPosition);
}

void Folie::Player::pass(Ball ^ball)
{
	auto distanceToTheBall = GB::distanceBetweenTwoPoints3D(ball->pos_x, ball->pos_y, ball->pos_z, pos_x, pos_y, pos_z);

	if (distanceToTheBall < 1)
		ball->moveTo(GB::ePosition::p3);
}

void Folie::Player::serve(Random ^rnd, Ball ^ball)
{
	currentArea = GB::eArea::a1S;
	hit(rnd, ball);
}

void Folie::Player::hit(Random ^rnd, Ball ^ball)
{
	auto distanceToTheBall = GB::distanceBetweenTwoPoints3D(ball->pos_x, ball->pos_y, ball->pos_z, pos_x, pos_y, pos_z);

	if (distanceToTheBall < 1)
	{
		auto target = GB::selectRandomPosition(rnd);
		ball->moveTo(target);
	}
}

void Folie::Player::lookAtTheBall(Ball ^ball)
{
	rot_y = GB::angleBetweenTwoPoints2D(ball->pos_x, ball->pos_z, pos_x, pos_z);
}
