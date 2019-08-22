/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.h"
#include "Team.h"
#include "BawiTim.h"
#include "Gruppo1.h"

using namespace System;

namespace Folie
{
	public ref class Game :BaseEvent
	{
	private:
		Random ^rnd;

		UInt16 matchPoints;
		UInt16 teamAPoints, teamBPoints;

		bool ballHaveHittedTheFloor;
		bool gameStop;

	public:
		Team ^teamA, ^teamB;
		Ball ^ball;


	private:
		bool isGameFinished();
		void serving();
		void pass(Team ^receivingTeam);
		void team_bubbleUp(GB::eEvent e);

	public:
		delegate void gameFinishedHandler();
		event gameFinishedHandler ^event_gameFinished;


	public:
		Game(UInt16 matchPoints_);

		void start();
		void stop();
		

	internal:
		void propagateEvent(GB::eEvent e, Object ^p1) override;


	};
}