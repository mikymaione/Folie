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
		System::Collections::IEnumerator ^iter;
		System::Action ^runMethod, ^recursive;
		Enums::eMode when_runMethod;

		ref class GenericEnumerator :System::Collections::IEnumerator
		{
		private:
			System::Collections::IEnumerator ^iter;
			System::Action ^runMethod, ^recursive;
			Enums::eMode when_runMethod;

		public:
			GenericEnumerator(Enums::eMode when_runMethod, System::Action ^runMethod, System::Collections::IEnumerator ^iter, System::Action ^recursive)
			{
				this->when_runMethod = when_runMethod;
				this->runMethod = runMethod;
				this->recursive = recursive;
				this->iter = iter;
			};

			virtual bool MoveNext()
			{
				switch (when_runMethod)
				{
				case Enums::eMode::First:
					if (runMethod != nullptr)
					{
						runMethod();
						runMethod = nullptr;
					}

					break;
				}

				auto more_elements_available = iter->MoveNext();

				if (!more_elements_available)
				{
					switch (when_runMethod)
					{
					case Enums::eMode::Last:
						if (runMethod != nullptr)
						{
							runMethod();
							runMethod = nullptr;
						}

						break;
					}

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
				iter->Reset();
			};

			virtual property Object ^Current
			{
				Object ^get()
				{
					return iter->Current;
				}
			};

		};

	public:
		GenericEnumerable(Enums::eMode when_runMethod, System::Action ^runMethod, System::Collections::IEnumerator ^iter, System::Action ^recursive)
		{
			this->when_runMethod = when_runMethod;
			this->runMethod = runMethod;
			this->recursive = recursive;
			this->iter = iter;
		}

		virtual System::Collections::IEnumerator ^GetEnumerator()
		{
			return gcnew GenericEnumerator(when_runMethod, runMethod, iter, recursive);
		};

	};
}