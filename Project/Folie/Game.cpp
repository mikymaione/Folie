/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Game.hpp"
#include "REF.hpp"

Folie::Game::Game()
{
	GB::init();
	currentTeamRotation = Enums::eRotateTeam::null;

	SM = gcnew AI::SM();

	auto S_serve = SM->addState(
		this,
		gcnew Action(this, &Game::serve)
	);

	auto S_playersTakePositionsOnTheField = SM->addState(
		this,
		gcnew Action(this, &Game::playersTakePositionsOnTheField),
		gcnew Func<bool>(this, &Game::arePlayersInPosition),
		S_serve
	);

	auto S_startAllUnityEntities = SM->addState(
		this,
		gcnew Func<bool>(this, &Game::started),
		gcnew Action(this, &Game::startTheMatch),
		S_playersTakePositionsOnTheField
	);

	SM->current = S_startAllUnityEntities;
}

bool Folie::Game::started()
{
	return (_started && ball->started() && teamA->started() && teamB->started());
}

// Unity
void Folie::Game::Start()
{
	REF::ball = ball;

	REF::teamA = teamA;
	REF::teamB = teamB;

	REF::teams = gcnew array<Team ^> {teamA, teamB};

	REF::game = this;

	_started = true;

	SM->run(SM->current);
}

// Unity
void Folie::Game::Update()
{
	scoreTeamA->text = teamA->name + ": " + teamAPoints;
	scoreTeamB->text = teamB->name + ": " + teamBPoints;

	touchTeamA->text = "Touch: " + teamA->getTouch();
	touchTeamB->text = "Touch: " + teamB->getTouch();
}

void Folie::Game::startTheMatch()
{
	gameStop = false;
	gameStarted = true;

	teamAPoints = 0;
	teamBPoints = 0;

	startRound(Enums::eRotateTeam::null);
}

void Folie::Game::stopTheMatch()
{
	gameStop = true;
}

void Folie::Game::startRound(Enums::eRotateTeam rotate)
{
	this->currentTeamRotation = rotate;
}

void Folie::Game::playersTakePositionsOnTheField()
{
	switch (currentTeamRotation)
	{
	case Enums::eRotateTeam::TeamA:
		teamA->moveToNextPosition();
		teamB->playersTakePositionsOnTheField();
		break;
	case Enums::eRotateTeam::TeamB:
		teamB->moveToNextPosition();
		teamA->playersTakePositionsOnTheField();
		break;
	default:
		teamA->playersTakePositionsOnTheField();
		teamB->playersTakePositionsOnTheField();
		break;
	}
}

bool Folie::Game::arePlayersInPosition()
{
	for each (auto t in REF::teams)
		if (!t->arePlayersInPosition())
			return false;

	return true;
}

void Folie::Game::serve()
{
	if (currentServingTeam == nullptr)
	{
		auto t = GB::rndUInt16(1, 2);
		currentServingTeam = (t == 1 ? teamA : teamB);
	}

	receivingTeam = (currentServingTeam == teamA ? teamB : teamA);

	currentServingTeam->serving = true;
	receivingTeam->serving = false;

	currentServingTeam->serve();
	ballOnTheGround = false;

	receivingTeam->playersTakePositionInReception();
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
	else
		return false;
}

void Folie::Game::ballOnGround(Enums::eField field_in_cui_cade, Player ^playerTouch)
{
	if (!ballOnTheGround)
	{
		ballOnTheGround = true;

		teamA->reset();
		teamB->reset();

		auto t = playerTouch->team;
		auto rotate = Enums::eRotateTeam::null;
		auto prevServingTeam = currentServingTeam;

		if (field_in_cui_cade == Enums::eField::out && t->Equals(teamA))
			rotate = assignPointToTeam(prevServingTeam, Enums::eTeam::B);
		else if (field_in_cui_cade == Enums::eField::out && t->Equals(teamB))
			rotate = assignPointToTeam(prevServingTeam, Enums::eTeam::A);
		else if (t->field == field_in_cui_cade && t->Equals(teamA))
			rotate = assignPointToTeam(prevServingTeam, Enums::eTeam::B);
		else if (t->field != field_in_cui_cade && t->Equals(teamA))
			rotate = assignPointToTeam(prevServingTeam, Enums::eTeam::A);
		else if (t->field == field_in_cui_cade && t->Equals(teamB))
			rotate = assignPointToTeam(prevServingTeam, Enums::eTeam::A);
		else if (t->field != field_in_cui_cade && t->Equals(teamB))
			rotate = assignPointToTeam(prevServingTeam, Enums::eTeam::B);

		if (isGameFinished())
			stopTheMatch();
		else
			startRound(rotate);
	}
}

Folie::Enums::eRotateTeam Folie::Game::assignPointToTeam(Team ^prevServingTeam, Enums::eTeam t)
{
	switch (t)
	{
	case Enums::eTeam::A:
		teamAPoints++;
		currentServingTeam = teamA;
		break;
	case Enums::eTeam::B:
		teamBPoints++;
		currentServingTeam = teamB;
		break;
	}

	auto rotate = (currentServingTeam->Equals(prevServingTeam) ? Enums::eRotateTeam::null : GB::teamToRotate(t));

	return rotate;
}