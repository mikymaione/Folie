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
	ref class GB  abstract sealed
	{

	public:

		enum class eArea
		{
			a4O, a4, a3, a2, a2E,
			a5O, a5, a6, a1, a1E,
			a5OS, a5S, a6S, a1S, a1ES
		};

		enum class ePosition
		{
			p4, p3, p2,
			p5, p6, p1
		};


		static eArea getAreaFromPosition(ePosition current);
		static ePosition getNextRotationPosition(ePosition current);
		static ePosition selectRandomPosition(Random ^rnd);

		static double angleBetweenTwoPoints2D(double x1, double z1, double x2, double z2);
		static double distanceBetweenTwoPoints3D(double x1, double y1, double z1, double x2, double y2, double z2);

	};
}