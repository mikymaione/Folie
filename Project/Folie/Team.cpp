/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Team.h"
#include "REF.h"

Folie::Team::Team()
{
	waiter = gcnew Waiter();
}

void Folie::Team::Start()
{
	players = gcnew array<Player ^> {P1, P2, P3, P4, P5, P6};

	for each (auto p in players)
		p->team = this;
}

Folie::Player ^Folie::Team::getPlayerAtPosition(Enums::ePosition position)
{
	for each (auto p in players)
		if (p->currentPosition == position)
			return p;
}

List<Folie::Player ^> ^Folie::Team::getPlayersWithRole(Enums::eRole role)
{
	auto p_in_role = gcnew List<Player ^>(6);

	for each (auto p in players)
		if (p->role == role)
			p_in_role->Add(p);

	return p_in_role;
}

Folie::Player ^Folie::Team::getPlayerWithRole(Player ^mySelf, Enums::eRole search_role, Enums::eCourt court)
{
	auto players_in_role = getPlayersWithRole(search_role);

	for each (auto p in players_in_role)
		if (!p->Equals(mySelf))
			if (p->getCurrentCourt() == court)
				return p;

	for each (auto p in players_in_role)
		if (!p->Equals(mySelf))
			return p;

	for each (auto p in players)
		if (!p->Equals(mySelf))
			return p;
}

void Folie::Team::giocatoriPrenderePosizioniInCampo()
{
	for each (auto p in players)
	{
		auto d = GB::getCoordinatesFromPosition(campo, p->currentPosition);

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

void Folie::Team::moveToNextPosition()
{
	for each (auto p in players)
		p->moveToNextPosition();
}

void Folie::Team::lookAtOpponent()
{
	for each (auto p in players)
		p->lookAtAnOpponent();
}

void Folie::Team::serve()
{
	for each (auto p in players)
		if (p->currentPosition == Enums::ePosition::p1)
		{			
			p->serveRitual();
			break;
		}
}