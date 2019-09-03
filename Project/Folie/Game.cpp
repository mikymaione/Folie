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
	GB::init();
	waiter = gcnew CoroutineQueue();
}

void Folie::Game::Start()
{
	REF::ball = ball;

	REF::teamA = teamA;
	REF::teamB = teamB;

	REF::teams = gcnew array<Team ^> {teamA, teamB};

	REF::game = this;

	waiter->waitAndCall(
		this,
		REF::w4s(2),
		gcnew Action(this, &Game::startTheMatch)
	);
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

	startRound(Enums::eRotate::null);
}

void Folie::Game::stopTheMatch()
{
	gameStop = true;
}

void Folie::Game::startRound(Enums::eRotate rotate)
{
	waiter->callAndWait(
		this,
		gcnew Action<Enums::eRotate>(this, &Game::giocatoriPrenderePosizioniInCampo),
		gcnew array<Enums::eRotate ^> {rotate},
		REF::wUntil(gcnew System::Func<bool>(this, &Game::giocatoriInPosizione))
	);

	waiter->callAndWait(
		this,
		gcnew Action(this, &Game::serve),
		REF::w4s(0)
	);
}

void Folie::Game::giocatoriPrenderePosizioniInCampo(Enums::eRotate rotate)
{
	switch (rotate)
	{
	case Folie::Enums::eRotate::TeamA:
		REF::teamA->moveToNextPosition();
		REF::teamB->giocatoriPrenderePosizioniInCampo();
		break;
	case Folie::Enums::eRotate::TeamB:
		REF::teamB->moveToNextPosition();
		REF::teamA->giocatoriPrenderePosizioniInCampo();
		break;
	default:
		REF::teamA->giocatoriPrenderePosizioniInCampo();
		REF::teamB->giocatoriPrenderePosizioniInCampo();
		break;
	}
}

bool Folie::Game::giocatoriInPosizione()
{
	for each (auto t in REF::teams)
		if (!t->giocatoriInPosizione())
			return false;

	return true;
}

void Folie::Game::serve()
{
	lookAtOpponent();

	if (currentServingTeam == nullptr)
	{
		auto t = GB::rndUInt16(1, 2);
		currentServingTeam = (t == 1 ? REF::teamB : REF::teamA);
	}

	currentServingTeam->serve();
	ballOnTheGround = false;
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
	for each (auto t in REF::teams)
		t->lookAtOpponent();
}

void Folie::Game::ballOnGround(Enums::eCampo campo_in_cui_cade, Player ^playerTouch)
{
	if (!ballOnTheGround)
	{
		ballOnTheGround = true;

		auto t = playerTouch->team;
		auto rotate = Enums::eRotate::null;
		auto prevServingTeam = currentServingTeam;

		if (campo_in_cui_cade == Enums::eCampo::fuori && t->Equals(teamA))
			rotate = assegnaPuntoATeam(prevServingTeam, Enums::eTeam::B);
		else if (campo_in_cui_cade == Enums::eCampo::fuori && t->Equals(teamB))
			rotate = assegnaPuntoATeam(prevServingTeam, Enums::eTeam::A);
		else if (t->campo == campo_in_cui_cade && t->Equals(teamA))
			rotate = assegnaPuntoATeam(prevServingTeam, Enums::eTeam::B);
		else if (t->campo != campo_in_cui_cade && t->Equals(teamA))
			rotate = assegnaPuntoATeam(prevServingTeam, Enums::eTeam::A);
		else if (t->campo == campo_in_cui_cade && t->Equals(teamB))
			rotate = assegnaPuntoATeam(prevServingTeam, Enums::eTeam::A);
		else if (t->campo != campo_in_cui_cade && t->Equals(teamB))
			rotate = assegnaPuntoATeam(prevServingTeam, Enums::eTeam::B);

		if (isGameFinished())
			stopTheMatch();
		else
			startRound(rotate);
	}
}

Folie::Enums::eRotate Folie::Game::assegnaPuntoATeam(Team ^prevServingTeam, Enums::eTeam t)
{
	switch (t)
	{
	case Folie::Enums::eTeam::A:
		teamAPoints++;
		currentServingTeam = teamA;
		break;
	case Folie::Enums::eTeam::B:
		teamBPoints++;
		currentServingTeam = teamB;
		break;
	}

	auto rotate = (currentServingTeam->Equals(prevServingTeam) ? Enums::eRotate::null : GB::teamToRotate(t));

	return rotate;
}