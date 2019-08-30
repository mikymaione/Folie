/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Game.h"
#include "REF.h"

Folie::Game::Game()
{
	GB::rnd = gcnew Random(DateTime::Now.Second);
	REF::waiter = gcnew Waiter();
}

void Folie::Game::Start()
{
	REF::ball = ball;

	REF::teamA = teamA;
	REF::teamB = teamB;

	REF::teams = gcnew array<Team ^> {teamA, teamB};

	REF::game = this;

	REF::waiter->waitAndCall(
		this,
		REF::w4s(2),
		gcnew Action(this, &Game::startTheMatch)
	);

	REF::waiter->Run();
}

void Folie::Game::Update()
{
	this->scoreTeamA->text = teamA->name + ": " + teamAPoints;
	this->scoreTeamB->text = teamB->name + ": " + teamBPoints;
}

void Folie::Game::startTheMatch()
{
	gameStop = false;
	gameStarted = true;

	teamAPoints = 0;
	teamBPoints = 0;

	startRound();
}

void Folie::Game::stopTheMatch()
{
	gameStop = true;
}

void Folie::Game::startRound()
{
	REF::waiter->clear();

	REF::waiter->callAndWait(
		this,
		gcnew Action(this, &Game::giocatoriPrenderePosizioniInCampo),
		REF::wUntil(gcnew System::Func<bool>(this, &Game::giocatoriInPosizione))
	);

	REF::waiter->callAndWait(
		this,
		gcnew Action(this, &Game::serve),
		REF::w4s(0)
	);

	REF::waiter->Run();
}

void Folie::Game::giocatoriPrenderePosizioniInCampo()
{
	REF::teamA->giocatoriPrenderePosizioniInCampo();
	REF::teamB->giocatoriPrenderePosizioniInCampo();

	lookAtOpponent();
}

bool Folie::Game::giocatoriInPosizione()
{
	return REF::teamA->giocatoriInPosizione() && REF::teamB->giocatoriInPosizione();
}

void Folie::Game::serve()
{
	lookAtOpponent();

	if (currentServingTeam == nullptr)
	{
		auto t = GB::rnd->Next(1, 3);

		currentServingTeam = (t == 1 ? REF::teamB : REF::teamA);
	}

	currentServingTeam->serve();
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

void Folie::Game::lookAtOpponent()
{
	REF::teamA->lookAtOpponent();
	REF::teamB->lookAtOpponent();
}

void Folie::Game::ballOnGround(Team ^t)
{
	if (t->Equals(teamA))
	{
		teamBPoints++;
		currentServingTeam = teamB;
	}
	else if (t->Equals(teamB))
	{
		teamAPoints++;
		currentServingTeam = teamA;
	}

	if (isGameFinished())
		stopTheMatch();
	else
		startRound();
}