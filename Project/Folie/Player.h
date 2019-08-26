/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.h"
#include "Ball.h"

#using <UnityEngine.AIModule.dll> as_friend
#using <Yielder.dll> as_friend

using namespace System::Collections::Generic;

namespace Folie
{
	public ref class Player abstract :UnityEngine::MonoBehaviour
	{
	private:
		GB::ePosition targetChoosen;
		UnityEngine::AI::NavMeshAgent ^agent;

	internal:
		UnityEngine::Transform ^mano;

		GB::ePosition currentPosition;
		GB::eArea currentArea, startingArea;
		GB::eCampo campo;

		bool lookingAtTheBall;

	public:
		String ^name;

		GB::ePosition startingPosition;
		GB::eRole role;

		GB::eTeam team;

	private:
		void Start();
		void Update();

	public:
		void move();
		void moveToPosition(GB::ePosition position);
		void moveToNextPosition();
		void moveTo(UnityEngine::Vector3 ^position);
		void moveTo(float pos_x, float pos_z);

		void pass_mode();
		void serve();
		void hit(GB::ePosition target);
		void hit();

		void lookAtAnOpponent();
		void lookAt(float x, float y);
		void lookAt(UnityEngine::Vector2 ^dest);

	};
}