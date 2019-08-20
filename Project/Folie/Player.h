/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.h"
#include "Ball.h"

using namespace System;

namespace Folie
{
	public ref class Player
	{
	public:
		String ^name;
		GB::ePosition startingPosition, currentPosition;
		GB::eArea currentArea;
		GB::eRole role;
		GB::eCampo campo;

		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;

	private:
		void moveCampoUp();
		void moveCampoDown();

	public:
		Player(String ^name_, GB::eCampo campo_, GB::ePosition startingPosition_, GB::eRole role_);

		void setPos(float x, float y, float z);
		void setPos(float x, float z);

		void move();
		void moveToNextPosition();
		void pass(Ball ^ball);
		void serve(Random ^rnd, Ball ^ball);
		void hit(Random ^rnd, Ball ^ball);
		void lookAtTheBall(Ball ^ball);

	};
}