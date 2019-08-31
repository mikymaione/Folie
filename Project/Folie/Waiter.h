/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#using <UnityEngine.CoreModule.dll> as_friend

#include "Work.h"
#include "GenericEnumerable.h"

using namespace System;

namespace Folie
{
	ref class Waiter
	{
	private:
		bool running;
		System::Collections::Generic::Queue<Work^> ^works;

		void runNextWork()
		{
			if (works->Count > 0)
			{
				running = true;

				auto work = works->Dequeue();

				auto ge = gcnew GenericEnumerable(
					work,
					gcnew Action(this, &Waiter::runNextWork)
				);

				work->this_->StartCoroutine(ge->GetEnumerator());
			}
			else
			{
				running = false;
			}
		}

		void enqueueWork(Enums::eSequence sequence, UnityEngine::MonoBehaviour ^this_, Delegate ^nome_funzione, array<Object ^> ^parametri, System::Collections::IEnumerator ^waiter)
		{
			auto work = gcnew Work(sequence, this_, nome_funzione, parametri, waiter);
			works->Enqueue(work);

			Run();
		};

		void Run()
		{
			if (!running)
				runNextWork();
		};

	internal:
		Waiter()
		{
			running = false;
			works = gcnew System::Collections::Generic::Queue<Work^>();
		};

		void clear()
		{
			running = false;
			works->Clear();
		};

		void callAndWait(UnityEngine::MonoBehaviour ^this_, Delegate ^nome_funzione, array<Object ^> ^parametri, System::Collections::IEnumerator ^waiter)
		{
			enqueueWork(Enums::eSequence::callAndWait, this_, nome_funzione, parametri, waiter);
		};

		void callAndWait(UnityEngine::MonoBehaviour ^this_, Delegate ^nome_funzione, System::Collections::IEnumerator ^waiter)
		{
			callAndWait(this_, nome_funzione, nullptr, waiter);
		};

		void waitAndCall(UnityEngine::MonoBehaviour ^this_, System::Collections::IEnumerator ^waiter, Delegate ^nome_funzione, array<Object ^> ^parametri)
		{
			enqueueWork(Enums::eSequence::waitAndCall, this_, nome_funzione, parametri, waiter);
		};

		void waitAndCall(UnityEngine::MonoBehaviour ^this_, System::Collections::IEnumerator ^waiter, Delegate ^nome_funzione)
		{
			waitAndCall(this_, waiter, nome_funzione, nullptr);
		};

	};
}