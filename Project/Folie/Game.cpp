/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Game.h"

Folie::Game::Game(UInt16 matchPoints_)
{
	gameStop = false;

	matchPoints = matchPoints_;

	GB::rnd = gcnew Random();

	ball = gcnew Ball();
	GB::ball = ball;

	teamA = gcnew Teams::BawiTim(GB::eCampo::up);
	teamB = gcnew Teams::Gruppo1(GB::eCampo::down);

	teamA->event_bubbleUp += gcnew eBubbleUpEvent(this, &Game::team_bubbleUp);
	teamB->event_bubbleUp += gcnew eBubbleUpEvent(this, &Game::team_bubbleUp);
}

void Folie::Game::stop()
{
	gameStop = true;
}

void Folie::Game::start()
{
	serving_ = false;

	teamAPoints = 0;
	teamBPoints = 0;

	teamA->destinationReached = 0;
	teamB->destinationReached = 0;

	teamA->propagateEvent(GB::eEvent::giocatoriPrenderePosizioniInCampo);
	teamB->propagateEvent(GB::eEvent::giocatoriPrenderePosizioniInCampo);
}

void Folie::Game::serving()
{
	if (!serving_)
	{
		serving_ = true;

		teamA->propagateEvent(GB::eEvent::lookAtTheBall);
		teamB->propagateEvent(GB::eEvent::lookAtTheBall);

		auto t = GB::rnd->Next(1, 3);

		auto currentReceivingTeam = (t == 2 ? teamA : teamB);
		auto currentServingTeam = (t == 1 ? teamA : teamB);
		auto currentAttackingTeam = currentServingTeam;

		currentServingTeam->propagateEvent(GB::eEvent::takeTheBall);
	}
}

void Folie::Game::pass(Team ^receivingTeam)
{
	while (!ballHaveHittedTheFloor)
		for (UInt16 i = 0; i < 6; i++)
		{
			auto p = receivingTeam->players[i];
			p->pass(ball);
		}
}

bool Folie::Game::isGameFinished()
{
	if (gameStop)
		return true;
	else if (teamAPoints < matchPoints)
		return false;
	else if (teamBPoints < matchPoints)
		return false;
	else if (Math::Abs(teamAPoints - teamBPoints) > 1) // 25 - 27
		return true;
}

void Folie::Game::propagateEvent(GB::eEvent e)
{
	//
}

void Folie::Game::team_bubbleUp(GB::eEvent e)
{
	switch (e)
	{
	case Folie::GB::eEvent::giocatoriPrenderePosizioniInCampo_end:
		serving();
		break;
	}
}