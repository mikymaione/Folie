/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Team.h"
#include "REF.h"

void Folie::Team::Start()
{
	players = gcnew array<Player ^> {P1, P2, P3, P4, P5, P6};
}

Folie::Player ^Folie::Team::getPlayerAtPosition(Enums::ePosition position)
{
	for each (auto p in players)
		if (p->currentPosition == position)
			return p;
}

Folie::Player ^Folie::Team::getPlayerWithRole(Enums::eRole role)
{
	for each (auto p in players)
		if (p->role == role)
			return p;
}

void Folie::Team::giocatoriPrenderePosizioniInCampo()
{
	for each (auto p in players)
	{
		auto d = GB::getCoordinatesFromPosition(campo, p->startingPosition);

		p->campo = campo;
		p->moveTo_Async(d->x, d->y);
	}
}

bool Folie::Team::giocatoriInPosizione()
{
	for each (auto p in players)
		if (!p->inPosizione())
			return false;

	return true;
}

void Folie::Team::lookAtOpponent()
{
	for each (auto p in players)
		p->lookAtAnOpponent(0);
}

void Folie::Team::serve()
{
	for each (auto p in players)
		if (p->currentArea == Enums::eArea::a1)
		{
			p->serveRitual();
			break;
		}
}