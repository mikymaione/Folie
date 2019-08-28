/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "Ball.h"
#include "REF.h"

void Folie::Ball::attachToHand(String ^player_name)
{
	for each (auto p in REF::teamA->players)
		if (p->name->Equals(player_name))
		{
			inMano = p->mano;
			break;
		}
}

bool Folie::Ball::ballInHand()
{
	return inMano != nullptr;
}

void Folie::Ball::Start()
{
	rigidBody = GetComponent<UnityEngine::Rigidbody^>();
}

void Folie::Ball::Update()
{
	if (inMano != nullptr)
		transform->SetPositionAndRotation(inMano->position, inMano->rotation);
}

void Folie::Ball::moveTo(Enums::eCampo campo, Enums::ePosition position)
{
	inMano = nullptr;

	auto c = GB::getCoordinatesFromPosition(campo, position);
	auto p = UnityEngine::Vector3(c->x, 0, c->y);

	auto dir = p - transform->position;
	auto h = dir.y;
	dir.y = 0;

	auto dist = dir.magnitude;
	dir.y = dist;
	dist += h;

	auto vel = UnityEngine::Mathf::Sqrt(dist * UnityEngine::Physics::gravity.magnitude);

	rigidBody->velocity = vel * dir.normalized;
}