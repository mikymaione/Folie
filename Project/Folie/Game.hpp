/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.hpp"
#include "Team.hpp"
#include "SM.hpp"

#using <UnityEngine.UI.dll> as_friend

using namespace System;

namespace Folie
{
	public ref class Game :MonoBehaviourEX
	{
	private:
		Enums::eRotateTeam currentTeamRotation;
		UInt16 teamAPoints, teamBPoints;

		bool ballHaveHittedTheFloor;
		bool gameStop, gameStarted, ballOnTheGround;

		Team ^currentServingTeam, ^receivingTeam;
		AI::SM ^SM;

	public:
		UInt16 matchPoints;
		Ball ^ball;
		Team ^teamA, ^teamB;

		UnityEngine::UI::Text ^scoreTeamA, ^scoreTeamB, ^touchTeamA, ^touchTeamB, ^playerName, ^mineTeamA, ^mineTeamB;

	private:
		void Start();
		void Update();

		bool allInitComplete();

		bool isGameFinished();

		void serve();

		void playersTakePositionsOnTheField();
		bool arePlayersInPosition();

		Folie::Enums::eRotateTeam assignPointToTeam(Team ^prevServingTeam, Enums::eTeam t);

	internal:
		void ballOnGround(Enums::eField field_in_cui_cade, Player ^playerTouch);
		void startRound(Enums::eRotateTeam rotate);

	public:
		Game();

		void startTheMatch();
		void stopTheMatch();

	};
}