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
	currentTeamRotation = Enums::eRotateTeam::null;

	GB::init();
	createFSM();
}

// Unity
void Folie::Game::Start()
{
	REF::game = this;
	REF::ball = ball;

	REF::teamA = teamA;
	REF::teamB = teamB;
	REF::teams = gcnew array<Team ^> {teamA, teamB};

	teamASetsWin = 0;
	teamBSetsWin = 0;

	_started = true;

	SM->run(S_startAllUnityEntities);
}

// Unity
void Folie::Game::Update()
{
	scoreTeamA->text = teamA->name + ": " + teamAPoints;
	scoreTeamB->text = teamB->name + ": " + teamBPoints;

	touchTeamA->text = "Touch: " + teamA->getTouch();
	touchTeamB->text = "Touch: " + teamB->getTouch();
}

bool Folie::Game::started()
{
	return (_started && ball->started() && teamA->started() && teamB->started());
}

void Folie::Game::createFSM()
{
	SM = gcnew AI::SM(this);

	S_stopTheMatch = SM->addState(gcnew Action(this, &Game::stopTheMatch));

	S_rally = SM->addState();

	S_checkingSetsWon = SM->addState(gcnew Action(this, &Game::checkingSetsWon));

	S_checkingSetScore = SM->addState(gcnew Action(this, &Game::checkingSetScore));

	S_serve = SM->addState(gcnew Action(this, &Game::serve));

	S_playersTakePositionsOnTheField = SM->addState(
		gcnew Action(this, &Game::playersTakePositionsOnTheField),
		gcnew Func<bool>(this, &Game::arePlayersInPosition),
		S_serve
	);

	S_startAllUnityEntities = SM->addState(
		gcnew Func<bool>(this, &Game::started),
		gcnew Action(this, &Game::startTheMatch),
		S_playersTakePositionsOnTheField
	);

	S_receivingTeamRotateOnCourt = SM->addState(
		gcnew Func<bool>(this, &Game::started),
		S_checkingSetScore
	);

	S_serve->addTransition(S_serve, S_rally);

	S_rally->addTransition(S_rally, S_checkingSetScore);
	S_rally->addTransition(S_rally, S_receivingTeamRotateOnCourt);

	S_checkingSetScore->addTransition(gcnew AI::Transition(S_checkingSetScore, gcnew Func<bool>(this, &Game::isSetEnded), S_checkingSetsWon));
	S_checkingSetScore->addTransition(gcnew AI::Transition(S_checkingSetScore, gcnew Func<bool>(this, &Game::isNotSetEnded), S_playersTakePositionsOnTheField));

	S_checkingSetsWon->addTransition(gcnew AI::Transition(S_checkingSetsWon, gcnew Func<bool>(this, &Game::isGameEnded), S_stopTheMatch));
	S_checkingSetsWon->addTransition(gcnew AI::Transition(S_checkingSetsWon, gcnew Func<bool>(this, &Game::isNotGameEnded), S_playersTakePositionsOnTheField));
}

void Folie::Game::checkingSetScore()
{
	if (isSetEnded())
	{
		if (teamAPoints > teamBPoints)
			teamASetsWin++;
		else
			teamBSetsWin++;

		teamAPoints = 0;
		teamBPoints = 0;
	}
}

void Folie::Game::checkingSetsWon()
{
	//
}

void Folie::Game::startTheMatch()
{
	gameStop = false;
	gameStarted = true;

	teamAPoints = 0;
	teamBPoints = 0;

	this->currentTeamRotation = Enums::eRotateTeam::null;
}

void Folie::Game::stopTheMatch()
{
	gameStop = true;
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

bool Folie::Game::isSetEnded()
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

bool Folie::Game::isGameEnded()
{
	return (teamASetsWin > 2 || teamBSetsWin > 2);
}

bool Folie::Game::isNotSetEnded()
{
	return !isSetEnded();
}

bool Folie::Game::isNotGameEnded()
{
	return !isGameEnded();
}

void Folie::Game::pointMade(Enums::eField field_in_cui_cade, Player ^playerTouch)
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

		this->currentTeamRotation = rotate;

		SM->run(S_checkingSetScore);
		SM->run(S_playersTakePositionsOnTheField);
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