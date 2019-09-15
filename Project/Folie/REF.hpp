/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Unity/Wait4Seconds.hpp"
#include "CoroutineQueue.hpp"
#include "Ball.hpp"
#include "Team.hpp"
#include "Game.hpp"

#using <UnityEngine.AIModule.dll> as_friend
#using <UnityEngine.CoreModule.dll> as_friend
#using <UnityEngine.PhysicsModule.dll> as_friend

namespace Folie
{
	public ref class REF
	{
	public:
		static Ball ^ball;
		static Team ^teamA, ^teamB;
		static Game ^game;

		static array<Team ^> ^teams;

	internal:
		static System::Collections::IEnumerator ^w4ms(UInt16 milliseconds)
		{
			float ms = milliseconds;
			float s = ms / 1000.0f;

			if (s <= 0)
				s = 0.00001f;

			return gcnew Unity::Wait4Seconds(s);
		}

		static System::Collections::IEnumerator ^w4s(UInt16 seconds)
		{
			float s = seconds;

			if (s <= 0)
				s = 0.00001f;

			return gcnew Unity::Wait4Seconds(seconds);
		};

		static System::Collections::IEnumerator ^wUntil(System::Func<bool> ^fn)
		{
			return gcnew UnityEngine::WaitUntil(fn);
		};

		static System::Collections::IEnumerator ^wWhile(System::Func<bool> ^fn)
		{
			return gcnew UnityEngine::WaitWhile(fn);
		};

	};
}