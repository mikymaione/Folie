/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "CoRunner.h"
#include "GenericEnumerable.h"

Folie::CoRunnerItem::CoRunnerItem(Enums::eJob job, Enums::eMode mode, Action ^action, System::Collections::IEnumerator ^enu)
{
	this->job = job;
	this->mode = mode;
	this->action = action;
	this->enu = enu;
}

Folie::CoRunner::CoRunner(UnityEngine::MonoBehaviour ^mb)
{
	this->mb = mb;
	syncQueue = gcnew Queue<CoRunnerItem ^>();
}

void Folie::CoRunner::Enqueue(CoRunnerItem ^i)
{
	switch (i->job)
	{
	case Enums::eJob::Async:
		switch (i->mode)
		{
		case Enums::eMode::First:
			Start(i->action, i->enu);
			break;
		case Enums::eMode::Last:
			Start(i->enu, i->action);
			break;
		}
		break;
	case Enums::eJob::Sync:
		i->recursive = gcnew System::Action(this, &CoRunner::Run);
		syncQueue->Enqueue(i);
		break;
	}
}

void Folie::CoRunner::Run()
{
	if (syncQueue->Count > 0)
	{
		auto i = syncQueue->Dequeue();

		switch (i->mode)
		{
		case Enums::eMode::First:
			Start(i->action, i->enu, i->recursive);
			break;
		case Enums::eMode::Last:
			Start(i->enu, i->action, i->recursive);
			break;
		}
	}
}

void Folie::CoRunner::Start(Action ^runMethod, System::Collections::IEnumerator ^f_last, Action ^recursive)
{
	auto co = gcnew GenericEnumerable(Enums::eMode::First, runMethod, f_last, recursive);

	mb->StartCoroutine(co->GetEnumerator());
}

void Folie::CoRunner::Start(System::Collections::IEnumerator ^f_pre, Action ^runMethod, Action ^recursive)
{
	auto co = gcnew GenericEnumerable(Enums::eMode::Last, runMethod, f_pre, recursive);

	mb->StartCoroutine(co->GetEnumerator());
}

void Folie::CoRunner::Start(System::Action ^runMethod, System::Collections::IEnumerator ^f_last)
{
	Start(runMethod, f_last, nullptr);
}

void Folie::CoRunner::Start(System::Collections::IEnumerator ^f_pre, System::Action ^runMethod)
{
	Start(f_pre, runMethod, nullptr);
}