/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace Folie
{
	namespace AI
	{
		ref class Decision abstract
		{
		public:
			virtual void Execute() abstract;
		};

		ref class DT :Decision
		{
		public:
			Dictionary<Object^, Decision^> ^Switcher;
			Decision ^Positive, ^Negative;

			Delegate ^Test;
			array<Object ^> ^testParams;

		private:
			Object ^positiveVal;

		public:
			DT(Delegate ^Test)
			{
				this->Test = Test;
			}

			DT(Delegate ^Test, Object ^positiveVal) :DT(Test)
			{
				this->positiveVal = positiveVal;
			}

			void Execute() override
			{
				auto res = Test->DynamicInvoke(testParams);

				if (Switcher == nullptr)
				{
					if (positiveVal == nullptr || positiveVal->Equals(res))
					{
						if (Positive != nullptr)
							Positive->Execute();
					}
					else
					{
						if (Negative != nullptr)
							Negative->Execute();
					}
				}
				else
				{
					if (Switcher->ContainsKey(res))
						Switcher[res]->Execute();
				}
			}

			static Dictionary<Object^, Decision^> ^newSwitcher()
			{
				return gcnew Dictionary<Object^, Decision^>();
			}
		};
	}
}