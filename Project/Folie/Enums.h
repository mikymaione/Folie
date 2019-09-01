/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

namespace Folie
{
	public ref class Enums
	{
	public:
		literal float min_distance_to_hit = 1.5f;
		literal float min_distance_to_move_to_the_ball = 5.0f;

		literal float attack_angle = 50;
		literal float serve_angle = 32;
		literal float pass_angle = 70;

		enum class eRotate
		{
			TeamA, TeamB, null
		};

		enum class ePhase
		{
			serve, pass, set, attack, block, null
		};

		enum class eSequence
		{
			callAndWait, waitAndCall
		};

		enum class eTeam
		{
			A, B
		};

		enum class eCampo
		{
			up, down, fuori
		};

		enum class eCourt
		{
			front, back
		};

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

		enum class eRole
		{
			Setter, OutsideHitter, MiddleBlocker, Libero, Opposite
		};
	};
}