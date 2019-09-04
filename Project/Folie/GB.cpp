/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "GB.hpp"

void Folie::GB::init()
{
	rnd = gcnew Random(DateTime::Now.Second);
}

UInt16 Folie::GB::rndUInt16(UInt16 from_, UInt16 to_)
{
	return rnd->Next(from_, to_ + 1);
}

generic <class T> where T : UnityEngine::Component T Folie::GB::GetComponentsInChildren(UnityEngine::MonoBehaviour ^mb, String ^tag)
{
	auto elements = mb->GetComponentsInChildren<T>();

	for each (auto c in elements)
		if (c->tag->Equals(tag))
			return c;
}

bool Folie::GB::samePosition(UnityEngine::Vector3 a, UnityEngine::Vector3 b)
{
	return a.x == b.x && a.z == b.z;
}

Folie::Enums::eCampo Folie::GB::oppositeField(Enums::eCampo campo)
{
	switch (campo)
	{
	case Enums::eCampo::up:
		return Enums::eCampo::down;
	case Enums::eCampo::down:
		return Enums::eCampo::up;
	}
}

UnityEngine::Vector2 Folie::GB::getCoordinatesFromPosition(Enums::eCampo campo, Enums::ePosition position)
{
	auto area = getAreaFromPosition(position);

	return getCoordinatesFromArea(campo, area);
}

UnityEngine::Vector2 Folie::GB::getCoordinatesFromArea(Enums::eCampo campo, Enums::eArea area)
{
	switch (campo)
	{
	case Enums::eCampo::up:
		switch (area)
		{
		case Enums::eArea::a4O:
			return UnityEngine::Vector2(-1, 8);
		case Enums::eArea::a4:
			return UnityEngine::Vector2(2, 8);
		case Enums::eArea::a3:
			return UnityEngine::Vector2(5, 8);
		case Enums::eArea::a2:
			return UnityEngine::Vector2(8, 8);
		case Enums::eArea::a2E:
			return UnityEngine::Vector2(11, 8);
		case Enums::eArea::a5O:
			return UnityEngine::Vector2(-1, 4);
		case Enums::eArea::a5:
			return UnityEngine::Vector2(2, 3);
		case Enums::eArea::a6:
			return UnityEngine::Vector2(5, 3);
		case Enums::eArea::a1:
			return UnityEngine::Vector2(8, 3);
		case Enums::eArea::a1E:
			return UnityEngine::Vector2(11, 3);
		case Enums::eArea::a5OS:
			return UnityEngine::Vector2(-1, -1);
		case Enums::eArea::a5S:
			return UnityEngine::Vector2(2, -1);
		case Enums::eArea::a6S:
			return UnityEngine::Vector2(5, -1);
		case Enums::eArea::a1S:
			return UnityEngine::Vector2(8, -1);
		case Enums::eArea::a1ES:
			return UnityEngine::Vector2(11, -1);
		}
		break;
	case Enums::eCampo::down:
		switch (area)
		{
		case Enums::eArea::a4O:
			return UnityEngine::Vector2(11, 11);
		case Enums::eArea::a4:
			return UnityEngine::Vector2(8, 11);
		case Enums::eArea::a3:
			return UnityEngine::Vector2(5, 11);
		case Enums::eArea::a2:
			return UnityEngine::Vector2(2, 11);
		case Enums::eArea::a2E:
			return UnityEngine::Vector2(-1, 11);
		case Enums::eArea::a5O:
			return UnityEngine::Vector2(11, 16);
		case Enums::eArea::a5:
			return UnityEngine::Vector2(8, 16);
		case Enums::eArea::a6:
			return UnityEngine::Vector2(5, 16);
		case Enums::eArea::a1:
			return UnityEngine::Vector2(2, 16);
		case Enums::eArea::a1E:
			return UnityEngine::Vector2(-1, 16);
		case Enums::eArea::a5OS:
			return UnityEngine::Vector2(11, 20);
		case Enums::eArea::a5S:
			return UnityEngine::Vector2(8, 20);
		case Enums::eArea::a6S:
			return UnityEngine::Vector2(5, 20);
		case Enums::eArea::a1S:
			return UnityEngine::Vector2(2, 20);
		case Enums::eArea::a1ES:
			return UnityEngine::Vector2(-1, 20);
		}
		break;
	}

	throw gcnew Exception("case campo" + campo.ToString() + ",area: " + area.ToString() + " non gestito!");
}

Folie::Enums::eCampo Folie::GB::getCampoFromCoordinates(float x, float z)
{
	if (x < 1 || x >= 10)
		return Enums::eCampo::fuori;
	else
		return (z < 10 ? Enums::eCampo::up : Enums::eCampo::down);
}

Folie::Enums::eCourt Folie::GB::getCourtFromCoordinates(float z)
{
	if (z >= 7 && z < 13)
		return Enums::eCourt::front;
	else
		return Enums::eCourt::back;
}

Folie::Enums::eCourt Folie::GB::getCourtFromPosition(Enums::ePosition pos)
{
	switch (pos)
	{
	case Folie::Enums::ePosition::p4:
		return Enums::eCourt::front;
	case Folie::Enums::ePosition::p3:
		return Enums::eCourt::front;
	case Folie::Enums::ePosition::p2:
		return Enums::eCourt::front;
	case Folie::Enums::ePosition::p5:
		return Enums::eCourt::back;
	case Folie::Enums::ePosition::p6:
		return Enums::eCourt::back;
	case Folie::Enums::ePosition::p1:
		return Enums::eCourt::back;
	}
}

Folie::Enums::eArea Folie::GB::getAreaFromCoordinates(float x, float z)
{
	if (x < 1)
	{
		if (z < 1)
			return Enums::eArea::a5OS;
		else if (z >= 1 && z <= 6)
			return Enums::eArea::a5O;
		else if (z >= 7 && z <= 9)
			return Enums::eArea::a4O;
		else if (z >= 10 && z <= 12)
			return Enums::eArea::a2E;
		else if (z >= 13 && z <= 18)
			return Enums::eArea::a1E;
		else if (z > 18)
			return Enums::eArea::a1ES;
	}
	else if (x >= 1 && x <= 3)
	{
		if (z < 1)
			return Enums::eArea::a5S;
		else if (z >= 1 && z <= 6)
			return Enums::eArea::a5;
		else if (z >= 7 && z <= 9)
			return Enums::eArea::a4;
		else if (z >= 10 && z <= 12)
			return Enums::eArea::a2;
		else if (z >= 13 && z <= 18)
			return Enums::eArea::a1;
		else if (z > 18)
			return Enums::eArea::a1S;
	}
	else if (x >= 4 && x <= 6)
	{
		if (z < 1)
			return Enums::eArea::a6S;
		else if (z >= 1 && z <= 6)
			return Enums::eArea::a6;
		else if (z >= 7 && z <= 9)
			return Enums::eArea::a3;
		else if (z >= 10 && z <= 12)
			return Enums::eArea::a3;
		else if (z >= 13 && z <= 18)
			return Enums::eArea::a6;
		else if (z > 18)
			return Enums::eArea::a6S;
	}
	else if (x >= 7 && x <= 9)
	{
		if (z < 1)
			return Enums::eArea::a1S;
		else if (z >= 1 && z <= 6)
			return Enums::eArea::a1;
		else if (z >= 7 && z <= 9)
			return Enums::eArea::a2;
		else if (z >= 10 && z <= 12)
			return Enums::eArea::a4;
		else if (z >= 13 && z <= 18)
			return Enums::eArea::a5;
		else if (z > 18)
			return Enums::eArea::a5S;
	}
	else if (x > 9)
	{
		if (z < 1)
			return Enums::eArea::a1ES;
		else if (z >= 1 && z <= 6)
			return Enums::eArea::a1E;
		else if (z >= 7 && z <= 9)
			return Enums::eArea::a2E;
		else if (z >= 10 && z <= 12)
			return Enums::eArea::a4O;
		else if (z >= 13 && z <= 18)
			return Enums::eArea::a5O;
		else if (z > 18)
			return Enums::eArea::a5OS;
	}
}

Folie::Enums::eArea Folie::GB::getAreaFromPosition(Enums::ePosition current)
{
	switch (current)
	{
	case Enums::ePosition::p4:
		return Enums::eArea::a4;
	case Enums::ePosition::p3:
		return Enums::eArea::a3;
	case Enums::ePosition::p2:
		return Enums::eArea::a2;
	case Enums::ePosition::p5:
		return Enums::eArea::a5;
	case Enums::ePosition::p6:
		return Enums::eArea::a6;
	case Enums::ePosition::p1:
		return Enums::eArea::a1;
	}
}

Folie::Enums::ePosition Folie::GB::getNextRotationPosition(Enums::ePosition current)
{
	switch (current)
	{
	case Enums::ePosition::p1:
		return Enums::ePosition::p6;
	case Enums::ePosition::p2:
		return Enums::ePosition::p1;
	case Enums::ePosition::p3:
		return Enums::ePosition::p2;
	case Enums::ePosition::p4:
		return Enums::ePosition::p3;
	case Enums::ePosition::p5:
		return Enums::ePosition::p4;
	case Enums::ePosition::p6:
		return Enums::ePosition::p5;
	}
}

Folie::Enums::ePosition Folie::GB::selectRandomPosition(Enums::eCourt court)
{
	auto i = rnd->Next(1, 4);

	switch (court)
	{
	case Folie::Enums::eCourt::front:
		switch (i)
		{
		case 1:
			return Enums::ePosition::p4;
		case 2:
			return Enums::ePosition::p3;
		case 3:
			return Enums::ePosition::p2;
		}
	case Folie::Enums::eCourt::back:
		switch (i)
		{
		case 1:
			return Enums::ePosition::p5;
		case 2:
			return Enums::ePosition::p6;
		case 3:
			return Enums::ePosition::p1;
		}
	}
}

Folie::Enums::ePosition Folie::GB::selectRandomPosition()
{
	auto i = rnd->Next(1, 7);

	switch (i)
	{
	case 1:
		return Enums::ePosition::p1;
	case 2:
		return Enums::ePosition::p2;
	case 3:
		return Enums::ePosition::p3;
	case 4:
		return Enums::ePosition::p4;
	case 5:
		return Enums::ePosition::p5;
	case 6:
		return Enums::ePosition::p6;
	}
}

Folie::Enums::eCourt Folie::GB::selectRandomCourt()
{
	auto r = rndUInt16(1, 2);

	switch (r)
	{
	case 1:
		return Enums::eCourt::back;
	case 2:
		return Enums::eCourt::front;
	}
}

Folie::Enums::eRotate Folie::GB::teamToRotate(Enums::eTeam t)
{
	switch (t)
	{
	case Folie::Enums::eTeam::A:
		return Enums::eRotate::TeamA;
		break;
	case Folie::Enums::eTeam::B:
		return Enums::eRotate::TeamB;
		break;
	}
}