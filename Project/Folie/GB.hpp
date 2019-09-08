/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#using <UnityEngine.CoreModule.dll> as_friend

#include "Enums.hpp"

using namespace System;

namespace Folie
{
	public ref class GB abstract sealed
	{
	private:
		static Random ^rnd;

	public:
		static void init();

		static UInt16 rndUInt16(UInt16 from_, UInt16 to_);
		static Int16 rndInt16(Int16 from_, Int16 to_);

		static bool inTheFieldArea3D(UnityEngine::Vector3 pos);
		static bool outOfTheVisibleArea3D(UnityEngine::Vector3 pos);

		static bool samePosition2D(UnityEngine::Vector2 a, UnityEngine::Vector2 b);
		static bool samePosition3D(UnityEngine::Vector3 a, UnityEngine::Vector3 b);

		static bool canAttackJumping(Enums::eRole role);

		static Enums::eField oppositeField(Enums::eField field);

		static UnityEngine::Vector2 getCoordinates2DFromPosition(Enums::eField field, Enums::ePosition position);
		static UnityEngine::Vector2 getCoordinates2DFromArea(Enums::eField field, Enums::eArea area);

		static Enums::eArea getAttackArea(Enums::eCourt court, Enums::eRole role, Enums::ePosition position, UInt16 number_of_setters);

		static Enums::eField getCampoFromCoordinates(float x, float z);
		static Enums::eArea getAreaFromCoordinates(float x, float z);
		static Enums::eCourt getCourtFromCoordinates(float z);
		static Enums::eCourt getCourtFromPosition(Enums::ePosition pos);

		static Enums::eArea getAreaFromPosition(Enums::ePosition current);
		static Enums::ePosition getNextRotationPosition(Enums::ePosition current);

		static Enums::ePosition selectRandomPosition(Enums::eCourt court);
		static Enums::ePosition selectRandomPosition();

		static Enums::eCourt selectRandomCourt();

		static Enums::eRotateTeam teamToRotate(Enums::eTeam t);

		generic <class T> where T : UnityEngine::Component static T GetComponentsInChildren(UnityEngine::MonoBehaviour ^mb, String ^tag);

	};
}