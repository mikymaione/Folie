/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Globals.h"
#include "Team.h"
#include "Teams/BawiTim.h"
#include "Teams/Gruppo1.h"

using namespace System;

namespace Folie
{
	public ref class Game
	{
	private:

		Random ^rnd;

		UInt16 matchPoints;
		UInt16 teamAPoints, teamBPoints;

		eTeam currentServingTeam;

		Team ^teamA, ^teamB;


		bool isGameFinished()
		{
			if (teamAPoints < matchPoints)
				return false;
			else if (teamBPoints < matchPoints)
				return false;
			else if (Math::Abs(teamAPoints - teamBPoints) > 1) // 25 - 27
				return true;
		}

		void serving()
		{
			auto target = selectRandomPosition(rnd);
			shoot(ePosition::serving, target);
		}

		void shoot(ePosition from, ePosition to)
		{

		}

		void pass()
		{

		}

		void move()
		{

		}

	public:
		Game(UInt16 matchPoints_)
		{
			matchPoints = matchPoints_;

			rnd = gcnew Random();

			teamA = gcnew BawiTim();
			teamB = gcnew Gruppo1();
		}

		void start()
		{
			teamAPoints = 0;
			teamBPoints = 0;

			currentServingTeam = selectRandomTeam(rnd);

			while (!isGameFinished())
			{
				serving();

			}
		}


	};
}