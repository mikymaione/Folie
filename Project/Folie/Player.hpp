/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "GB.hpp"
#include "Ball.hpp"

#using <UnityEngine.AIModule.dll> as_friend

using namespace System::Collections::Generic;

namespace Folie
{
	ref class Team; // cross reference

	public ref class Player :UnityEngine::MonoBehaviour
	{
	private:
		bool jumping;

		CoroutineQueue ^waiter;

		Enums::ePosition targetChoosen;

		UnityEngine::AI::NavMeshAgent ^agent;
		UnityEngine::Rigidbody ^rigidBody;

		UnityEngine::Vector3 ^lookingAt;

		UnityEngine::Component ^goToElement;

	internal:
		UnityEngine::Transform ^mano;

		Enums::ePosition currentPosition;
		Enums::eArea currentArea, startingArea;
		Enums::eCampo campo;

		Enums::ePhase phase;

		Team ^team;

		float walkSpeed = 5;
		float runSpeed = 9;

	public:
		String ^name;

		Enums::ePosition startingPosition;
		Enums::eRole role;

	private:
		void Start();
		void Update();

		void moveTo_(float speed, float pos_x, float pos_z);

		void set_();
		void pass_();
		void attack_(Enums::ePosition target);
		void lookAt_(UnityEngine::Vector3 to_);

		void serve_(UnityEngine::Vector2 target);
		void serve();
		void takeTheBall();

		bool goToElementReached();

		void setJumping(bool j);

		void giocatorePrenderePosizioniInCampo(UnityEngine::Vector2 posizione);

	public:
		Player();

		float getDistanceFromBall();

		void giocatorePrenderePosizioniInRicezione();
		void giocatorePrenderePosizioniInCampo(Enums::eCampo campo_);

		bool inPassPosition();

		void move(float speed);
		void moveToNextPosition();
		void moveToPosition(float speed, Enums::ePosition position);
		void moveTo(float speed, UnityEngine::Component ^comp);
		void moveTo(float speed, UnityEngine::Vector3 position);
		void moveTo(float speed, float pos_x, float pos_z);
		void moveTo_Async(float speed, float pos_x, float pos_z);

		void pass();
		void set();

		void serveRitual();

		void attack();
		void attack(Enums::ePosition target);

		float calculateAngleOfAttack(Enums::ePosition target);

		void lookAtTheBall(bool looking);
		void lookAtAnOpponent();
		void lookAt(UInt16 seconds, UnityEngine::Vector2 dest);
		void lookAt(UInt16 seconds, float x, float z);
		void lookAt(UInt16 seconds, UnityEngine::Vector3 to_);

		Enums::eCourt getCurrentCourt();

	};
}