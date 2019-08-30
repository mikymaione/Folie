/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.h"

#using <UnityEngine.PhysicsModule.dll> as_friend

namespace Folie
{
	ref class Team; // cross reference

	public ref class Ball :UnityEngine::MonoBehaviour
	{
	private:
		UnityEngine::Rigidbody ^rigidBody;
		UnityEngine::Transform ^inMano;

		Team ^lastTeamTouch;
		bool hitted, ground;
		Enums::eCampo campoPrecedente;

	internal:
		UInt16 touch;

		Enums::eCampo getCampoAttuale();

		void attachToHand(String ^player_name);

		bool ballInHand();
		bool ballIsFlying();

	private:
		void Start();
		void Update();
		
		void OnCollisionEnter(UnityEngine::Collision collision);

		void move(Team ^t, Enums::eCampo campo, UnityEngine::Vector2 ^coordinate, float angle);

	public:
		void serve(Team ^t, Enums::eCampo campo, Enums::ePosition position);

		void hit(Team ^t, Enums::eCampo campo, Enums::ePosition position, float angle);
		void hit(Team ^t, Enums::eCampo campo, Enums::eArea area, float angle);
		void hit(Team ^t, Enums::eCampo campo, UnityEngine::Vector2 ^coordinate, float angle);

	};
}