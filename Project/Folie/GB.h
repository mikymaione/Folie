/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#using <UnityEngine.CoreModule.dll> as_friend

#include "Enums.h"

using namespace System;

namespace Folie
{
	public ref class GB abstract sealed
	{
	internal:
		static Random ^rnd;

	public:
		static bool samePosition(UnityEngine::Vector3 a, UnityEngine::Vector3 b);
		static Enums::eCampo oppositeField(Enums::eCampo campo);

		static UnityEngine::Vector2 ^getCoordinatesFromPosition(Enums::eCampo campo, Enums::ePosition position);
		static UnityEngine::Vector2 ^getCoordinatesFromArea(Enums::eCampo campo, Enums::eArea area);

		static Enums::eCampo getCampoFromCoordinates(float z);

		static Enums::eArea getAreaFromCoordinates(float x, float z);

		static Enums::eArea getAreaFromPosition(Enums::ePosition current);
		static Enums::ePosition getNextRotationPosition(Enums::ePosition current);
		static Enums::ePosition selectRandomPosition();

		generic <class T> where T : UnityEngine::Component static T GetComponentsInChildren(UnityEngine::MonoBehaviour ^mb, String ^tag);

	};
}