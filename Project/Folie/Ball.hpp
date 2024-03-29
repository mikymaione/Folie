/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.hpp"
#include "Unity/MonoBehaviourEX.hpp"

#using <UnityEngine.PhysicsModule.dll> as_friend

namespace Folie
{
	ref class CoroutineQueue; // cross reference
	ref class Player; // cross reference

	public ref class Ball sealed :Unity::MonoBehaviourEX
	{
	private:
		CoroutineQueue ^waiter;

		UnityEngine::Rigidbody ^rigidBody;
		UnityEngine::Transform ^inHand;

		bool hitted, hitting, ground;

	internal:
		Player ^lastPlayerTouch;

		Enums::eArea targetArea;

		UnityEngine::Vector2 destination2D, target2D;
		UnityEngine::Vector3 destination3D;

	internal:
		Enums::eField getActualField();

		void attachToHand(String ^player_name);

		bool ballInHand();
		bool ballIsFlying();

		bool started() override;

		Enums::eArea getTargetArea();

	private:
		void Start();
		void Update();

		void OnCollisionEnter(UnityEngine::Collision collision);

		void addForce(Player ^playerTouch, Enums::eField field, UnityEngine::Vector2 coordinate2D, float angle_Deg);

		void setHitting(bool hitting_);

		void ballOnTheFloor();

	public:
		Ball();

		void serve(Player ^playerTouch, Enums::eField field, UnityEngine::Vector2 coordinate2D);

		void hit(Player ^playerTouch, Enums::eField field, Enums::ePosition position, float angle_Deg);
		void hit(Player ^playerTouch, Enums::eField field, Enums::eArea area, float angle_Deg);
		void hit(Player ^playerTouch, Enums::eField field, UnityEngine::Vector2 coordinate2D, float angle_Deg);

	};
}