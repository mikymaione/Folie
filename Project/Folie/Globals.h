/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

using namespace System;

namespace Folie
{

	enum eTeam
	{
		TeamA, TeamB
	};

	enum ePosition
	{
		serving,
		p1, p2, p3, p4, p5, p6
	};


	eTeam selectRandomTeam(Random ^rnd)
	{
		auto i = rnd->Next(1, 3);

		switch (i)
		{
		case 1:
			return eTeam::TeamA;
		default:
			return eTeam::TeamB;
		}
	}

	ePosition getNextRotationPosition(ePosition current)
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

	ePosition selectRandomPosition(Random ^rnd)
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


}