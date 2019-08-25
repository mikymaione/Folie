/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "GB.h"

generic <class T> where T : UnityEngine::Component T Folie::GB::GetComponentsInChildren(UnityEngine::MonoBehaviour ^mb, String ^tag)
{
	auto elements = mb->GetComponentsInChildren<T>();

	for each (auto c in elements)
		if (c->tag->Equals(tag))
			return c;
}

Folie::GB::eCampo Folie::GB::oppositeField(eCampo campo)
{
	switch (campo)
	{
	case eCampo::up:
		return eCampo::down;
	case eCampo::down:
		return eCampo::up;
	}
}

UnityEngine::Vector2 ^Folie::GB::getCoordinatesFromPosition(eCampo campo, ePosition position)
{
	auto area = getAreaFromPosition(position);

	return getCoordinatesFromArea(campo, area);
}

UnityEngine::Vector2 ^Folie::GB::getCoordinatesFromArea(eCampo campo, eArea area)
{
	switch (campo)
	{
	case eCampo::up:
		switch (area)
		{
		case eArea::a4O:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a4:
			return gcnew UnityEngine::Vector2(2, 8);
		case eArea::a3:
			return gcnew UnityEngine::Vector2(5, 8);
		case eArea::a2:
			return gcnew UnityEngine::Vector2(8, 8);
		case eArea::a2E:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5O:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5:
			return gcnew UnityEngine::Vector2(2, 3);
		case eArea::a6:
			return gcnew UnityEngine::Vector2(5, 3);
		case eArea::a1:
			return gcnew UnityEngine::Vector2(8, 3);
		case eArea::a1E:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5OS:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5S:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a6S:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a1S:
			return gcnew UnityEngine::Vector2(8, -1);
		case eArea::a1ES:
			return gcnew UnityEngine::Vector2(0, 0);
		}
		break;
	case eCampo::down:
		switch (area)
		{
		case eArea::a4O:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a4:
			return gcnew UnityEngine::Vector2(8, 11);
		case eArea::a3:
			return gcnew UnityEngine::Vector2(5, 11);
		case eArea::a2:
			return gcnew UnityEngine::Vector2(2, 11);
		case eArea::a2E:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5O:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5:
			return gcnew UnityEngine::Vector2(8, 16);
		case eArea::a6:
			return gcnew UnityEngine::Vector2(5, 16);
		case eArea::a1:
			return gcnew UnityEngine::Vector2(2, 16);
		case eArea::a1E:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5OS:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a5S:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a6S:
			return gcnew UnityEngine::Vector2(0, 0);
		case eArea::a1S:
			return gcnew UnityEngine::Vector2(2, 20);
		case eArea::a1ES:
			return gcnew UnityEngine::Vector2(0, 0);
		}
		break;
	}
}

Folie::GB::eArea Folie::GB::getAreaFromPosition(ePosition current)
{
	switch (current)
	{
	case ePosition::p4:
		return eArea::a4;
	case ePosition::p3:
		return eArea::a3;
	case ePosition::p2:
		return eArea::a2;
	case ePosition::p5:
		return eArea::a5;
	case ePosition::p6:
		return eArea::a6;
	case ePosition::p1:
		return eArea::a1;
	}
}

Folie::GB::ePosition Folie::GB::getNextRotationPosition(ePosition current)
{
	switch (current)
	{
	case ePosition::p1:
		return ePosition::p6;
	case ePosition::p2:
		return ePosition::p1;
	case ePosition::p3:
		return ePosition::p2;
	case ePosition::p4:
		return ePosition::p3;
	case ePosition::p5:
		return ePosition::p4;
	case ePosition::p6:
		return ePosition::p5;
	}
}

Folie::GB::ePosition Folie::GB::selectRandomPosition()
{
	auto i = rnd->Next(1, 4);

	switch (i)
	{
	case 1:
		return ePosition::p5;
	case 2:
		return ePosition::p6;
	case 3:
		return ePosition::p1;
	}
}