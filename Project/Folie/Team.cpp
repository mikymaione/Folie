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
	auto x = -1;

	players = gcnew array<Player ^>(6);
	players[x += 1] = C1;
	players[x += 1] = C2;
	players[x += 1] = P1;
	players[x += 1] = P2;
	players[x += 1] = B1;
	players[x += 1] = B2;
}

void Folie::Team::Update()
{

}

Folie::Player ^Folie::Team::getPlayerAtPosition(GB::ePosition position)
{
	for each (auto p in players)
		if (p->currentPosition == position)
			return p;
}

Folie::Player ^Folie::Team::getPlayerWithRole(GB::eRole role)
{
	for each (auto p in players)
		if (p->role == role)
			return p;
}

bool Folie::Team::giocatoriPresePosizioniInCampo()
{
	for each (auto p in players)
		if (GB::getAreaFromCoordinates(p->transform->position.x, p->transform->position.z) != p->startingArea)
			return false;

	return true;
}

void Folie::Team::giocatoriPrenderePosizioniInCampo()
{
	for each (auto p in players)
	{
		p->campo = campo;
		p->moveToPosition(p->startingPosition);
	}
}

void Folie::Team::lookAtOpponent()
{
	for each (auto p in players)
		p->lookAtAnOpponent();
}

void Folie::Team::takeTheBall()
{
	for each (auto p in players)
		if (p->currentPosition == GB::ePosition::p1)
			p->moveTo(REF::ball->transform->position);
}