/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Player.hpp"

using namespace System;

namespace Folie
{
	public ref class Team sealed :MonoBehaviourEX
	{
	private:
		CoroutineQueue ^waiter;

		bool _lockTouch;
		UInt16 _touch;

	internal:
		Player ^playerThatSayMia;
		array<Player ^> ^players;

		UInt16 number_of_setters;
		bool serving;

	public:
		String ^name;
		Enums::eField field;

		Player ^P1, ^P2, ^P3, ^P4, ^P5, ^P6;

	private:
		void Start();
		void Update();

		void setMine();

		void unlockTouch();

	internal:
		bool started() override;

		UInt16 getTouch();
		void incTouch();
		void zeroTouch();

	public:
		Team();

		Player ^getPlayerAtPosition(Enums::ePosition position);

		List<Player ^> ^getPlayersWithRole(Enums::eRole role);
		Player ^getPlayerWithRole(Player ^mySelf, Enums::eRole search_role, Enums::eCourt court);

		Enums::ePosition getAnEmptyPosition(Enums::ePosition myPosition);

		void playersTakePositionInReception();
		void playersTakePositionsOnTheField();
		bool arePlayersInPosition();

		void moveToNextPosition();

		void lookAtOpponent();
		void serve();

		void reset();

	};
}