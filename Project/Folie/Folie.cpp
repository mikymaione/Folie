/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Folie.h"

Folie::Game::Game(UInt16 matchPoints_)
{
	matchPoints = matchPoints_;

	rnd = gcnew Random();

	teamA = gcnew BawiTim();
	teamB = gcnew Gruppo1();
}

void Folie::Game::serving(Team ^currentServingTeam)
{
	auto target = Folie::GB::selectRandomPosition(rnd);

	auto servingPlayer = currentServingTeam->getPlayerAtPosition(Folie::GB::ePosition::p1);
	servingPlayer->setPlayerServing();

	shoot(servingPlayer, target);
}

void Folie::Game::shoot(Player ^player, Folie::GB::ePosition to)
{

}

void Folie::Game::pass(Team ^receivingTeam)
{

}

void Folie::Game::move()
{

}

bool Folie::Game::isGameFinished()
{
	if (teamAPoints < matchPoints)
		return false;
	else if (teamBPoints < matchPoints)
		return false;
	else if (Math::Abs(teamAPoints - teamBPoints) > 1) // 25 - 27
		return true;
}

void Folie::Game::start()
{
	teamAPoints = 0;
	teamBPoints = 0;

	auto t = rnd->Next(1, 3);

	auto currentReceivingTeam = (t == 2 ? teamA : teamB);
	auto currentServingTeam = (t == 1 ? teamA : teamB);
	auto currentAttackingTeam = currentServingTeam;

	while (!isGameFinished())
	{
		serving(currentServingTeam);
		pass(currentReceivingTeam);
	}
}