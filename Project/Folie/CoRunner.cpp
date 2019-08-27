/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "CoRunner.h"
#include "GenericEnumerable.h"

Folie::CoRunner::CoRunner(UnityEngine::MonoBehaviour ^mb)
{
	this->mb = mb;

	asyncQueue = gcnew System::Collections::Generic::Queue<System::Collections::IEnumerator ^>();
	syncQueue = gcnew System::Collections::Generic::Queue<System::Collections::IEnumerator ^>();
}

void Folie::CoRunner::Enqueue(eJob job, System::Collections::IEnumerator ^cosa)
{
	switch (job)
	{
	case eJob::Async:
		asyncQueue->Enqueue(cosa);
		break;
	case eJob::Sync:
		syncQueue->Enqueue(syncCoRun(cosa));
		break;
	}
}

void Folie::CoRunner::Run()
{
	asyncRun();
	syncRun();
}

void Folie::CoRunner::syncRun()
{
	if (syncQueue->Count > 0)
		mb->StartCoroutine(syncQueue->Dequeue());
}

void Folie::CoRunner::asyncRun()
{
	while (asyncQueue->Count > 0)
		mb->StartCoroutine(asyncQueue->Dequeue());
}

System::Collections::IEnumerator ^Folie::CoRunner::syncCoRun(System::Collections::IEnumerator ^fun)
{
	auto enumerable = gcnew GenericEnumerable(
		GenericEnumerable::eMode::Last,
		gcnew System::Action(this, &CoRunner::asyncRun),
		fun
	);

	return enumerable->GetEnumerator();
}

void Folie::CoRunner::Start(System::Action ^runMethod, System::Collections::IEnumerator ^f_last)
{
	auto co = gcnew GenericEnumerable(GenericEnumerable::eMode::First, runMethod, f_last);

	mb->StartCoroutine(co->GetEnumerator());
}

void Folie::CoRunner::Start(System::Collections::IEnumerator ^f_pre, System::Action ^runMethod)
{
	auto co = gcnew GenericEnumerable(GenericEnumerable::eMode::Last, runMethod, f_pre);

	mb->StartCoroutine(co->GetEnumerator());
}