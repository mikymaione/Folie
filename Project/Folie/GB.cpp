/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "GB.h"


Folie::GB::eCampo Folie::GB::oppositeField(eCampo campo)
{
	switch (campo)
	{
	case Folie::GB::eCampo::up:
		return GB::eCampo::down;
	case Folie::GB::eCampo::down:
		return GB::eCampo::up;
	}
}

Point Folie::GB::getCoordinatesFromPosition(eCampo campo, ePosition position)
{
	auto area = getAreaFromPosition(position);

	return getCoordinatesFromArea(campo, area);
}

Point Folie::GB::getCoordinatesFromArea(eCampo campo, eArea area)
{
	switch (campo)
	{
	case Folie::GB::eCampo::up:
		switch (area)
		{
		case Folie::GB::eArea::a4O:
			return Point(0, 0);
		case Folie::GB::eArea::a4:
			return Point(2, 8);
		case Folie::GB::eArea::a3:
			return Point(5, 8);
		case Folie::GB::eArea::a2:
			return Point(8, 8);
		case Folie::GB::eArea::a2E:
			return Point(0, 0);
		case Folie::GB::eArea::a5O:
			return Point(0, 0);
		case Folie::GB::eArea::a5:
			return Point(2, 3);
		case Folie::GB::eArea::a6:
			return Point(5, 3);
		case Folie::GB::eArea::a1:
			return Point(8, 3);
		case Folie::GB::eArea::a1E:
			return Point(0, 0);
		case Folie::GB::eArea::a5OS:
			return Point(0, 0);
		case Folie::GB::eArea::a5S:
			return Point(0, 0);
		case Folie::GB::eArea::a6S:
			return Point(0, 0);
		case Folie::GB::eArea::a1S:
			return Point(8, -1);
		case Folie::GB::eArea::a1ES:
			return Point(0, 0);
		}
		break;
	case Folie::GB::eCampo::down:
		switch (area)
		{
		case Folie::GB::eArea::a4O:
			return Point(0, 0);
		case Folie::GB::eArea::a4:
			return Point(8, 11);
		case Folie::GB::eArea::a3:
			return Point(5, 11);
		case Folie::GB::eArea::a2:
			return Point(2, 11);
		case Folie::GB::eArea::a2E:
			return Point(0, 0);
		case Folie::GB::eArea::a5O:
			return Point(0, 0);
		case Folie::GB::eArea::a5:
			return Point(8, 16);
		case Folie::GB::eArea::a6:
			return Point(5, 16);
		case Folie::GB::eArea::a1:
			return Point(2, 16);
		case Folie::GB::eArea::a1E:
			return Point(0, 0);
		case Folie::GB::eArea::a5OS:
			return Point(0, 0);
		case Folie::GB::eArea::a5S:
			return Point(0, 0);
		case Folie::GB::eArea::a6S:
			return Point(0, 0);
		case Folie::GB::eArea::a1S:
			return Point(2, 20);
		case Folie::GB::eArea::a1ES:
			return Point(0, 0);
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
	auto i = rnd->Next(1, 7);

	switch (i)
	{
	case 1:
		return ePosition::p1;
	case 2:
		return ePosition::p2;
	case 3:
		return ePosition::p3;
	case 4:
		return ePosition::p4;
	case 5:
		return ePosition::p5;
	case 6:
		return ePosition::p6;
	}
}

double Folie::GB::distanceBetweenTwoPoints3D(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return Math::Sqrt(Math::Pow(x2 - x1, 2) + Math::Pow(y2 - y1, 2) + Math::Pow(z2 - z1, 2));
}
