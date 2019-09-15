/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include "Enums.hpp"

namespace Folie
{
	ref class GenericEnumerable :System::Collections::IEnumerable
	{
	private:
		Job ^work;
		System::Action ^callback_function;

		ref class GenericEnumerator :System::Collections::IEnumerator
		{
		private:
			Job ^work;
			System::Action ^callback_function;

		private:
			void invoke()
			{
				work->functionCalled = true;
				work->function->DynamicInvoke(work->parameters);
			};

		public:
			GenericEnumerator(Job ^work, System::Action ^callback_function)
			{
				this->work = work;
				this->callback_function = callback_function;
			};

			virtual bool MoveNext()
			{
				if (!work->functionCalled)
					if (work->sequence == Enums::eSequence::callAndWait)
						invoke();

				auto more_elements_available = work->waiter->MoveNext();

				if (!more_elements_available)
				{
					if (!work->functionCalled)
						if (work->sequence == Enums::eSequence::waitAndCall)
							invoke();

					if (callback_function != nullptr)
					{
						callback_function();
						callback_function = nullptr;
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
		GenericEnumerable(Job ^work, System::Action ^callback_function)
		{
			this->work = work;
			this->callback_function = callback_function;
		}

		virtual System::Collections::IEnumerator ^GetEnumerator()
		{
			return gcnew GenericEnumerator(work, callback_function);
		};

	};
}