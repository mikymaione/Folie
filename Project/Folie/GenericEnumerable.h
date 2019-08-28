/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Enums.h"

namespace Folie
{
	ref class GenericEnumerable :System::Collections::IEnumerable
	{
	private:
		Work ^work;
		System::Action ^recursive;

		ref class GenericEnumerator :System::Collections::IEnumerator
		{
		private:
			Work ^work;
			System::Action ^recursive;

		private:
			void invoke()
			{
				work->funzioneEseguita = true;

				try
				{
					auto thisType = work->this_->GetType();
					auto theMethod = thisType->GetMethod(work->nome_funzione, BindingFlags::NonPublic | BindingFlags::Public | BindingFlags::Instance);

					theMethod->Invoke(work->this_, work->parametri);
				}
				catch (System::Exception ^e)
				{
					UnityEngine::Debug::LogError(e->Message);
				}
			};

		public:
			GenericEnumerator(Work ^work, System::Action ^recursive)
			{
				this->work = work;
				this->recursive = recursive;
			};

			virtual bool MoveNext()
			{
				if (!work->funzioneEseguita)
					if (work->sequence == Enums::eSequence::callAndWait)
						invoke();

				auto more_elements_available = work->waiter->MoveNext();

				if (!more_elements_available)
				{
					if (!work->funzioneEseguita)
						if (work->sequence == Enums::eSequence::waitAndCall)
							invoke();

					if (recursive != nullptr)
					{
						recursive();
						recursive = nullptr;
					}
				}

				return more_elements_available;
			};

			virtual void Reset()
			{
				work->waiter->Reset();
			};

			virtual property Object ^Current
			{
				Object ^get()
				{
					return work->waiter->Current;
				}
			};

		};

	public:
		GenericEnumerable(Work ^work, System::Action ^recursive)
		{
			this->work = work;
			this->recursive = recursive;
		}

		virtual System::Collections::IEnumerator ^GetEnumerator()
		{
			return gcnew GenericEnumerator(work, recursive);
		};

	};
}