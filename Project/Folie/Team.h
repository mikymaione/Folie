/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Player.h"

using namespace System;

namespace Folie
{
	public ref class Team abstract :UnityEngine::MonoBehaviour
	{
	internal:
		array<Player ^> ^players;

	public:
		String ^name;
		GB::eCampo campo;

		Player ^P1, ^P2;
		Player ^C1, ^C2;
		Player ^B1, ^B2;

	private:
		void Start();
		void Update();

	public:
		Player ^getPlayerAtPosition(GB::ePosition position);
		Player ^getPlayerWithRole(GB::eRole role);

		bool giocatoriPresePosizioniInCampo();
		void giocatoriPrenderePosizioniInCampo();
		void lookAtOpponent();
		void takeTheBall();

	};
}