/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

namespace Folie
{
	ref class GenericEnumerable :System::Collections::IEnumerable
	{
	public:
		enum class eMode
		{
			First, Last
		};

	private:
		System::Collections::IEnumerator ^iter;
		System::Action ^runMethod;
		eMode mode;

		ref class GenericEnumerator :System::Collections::IEnumerator
		{
		private:
			System::Collections::IEnumerator ^iter;
			System::Action ^runMethod;
			eMode mode;

		public:
			GenericEnumerator(eMode mode, System::Action ^runMethod, System::Collections::IEnumerator ^iter)
			{
				this->mode = mode;
				this->runMethod = runMethod;
				this->iter = iter;
			};

			virtual bool MoveNext()
			{
				auto more_elements_available = iter->MoveNext();

				switch (mode)
				{
				case eMode::Last:
					if (!more_elements_available)
						runMethod();

					break;
				}

				return more_elements_available;
			};

			virtual void Reset()
			{
				switch (mode)
				{
				case eMode::First:
					runMethod();
					break;
				}

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
		GenericEnumerable(eMode mode, System::Action ^runMethod, System::Collections::IEnumerator ^iter)
		{
			this->mode = mode;
			this->runMethod = runMethod;
			this->iter = iter;
		}

		virtual System::Collections::IEnumerator ^GetEnumerator()
		{
			return gcnew GenericEnumerator(mode, runMethod, iter);
		};

	};
}