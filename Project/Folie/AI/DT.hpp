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
		interface class IDTNode
		{
			void Execute();
		};

		generic <class T> ref class DT :IDTNode
		{
		private:
			Dictionary<T, IDTNode^> ^decisions, ^notDecisions;

			Delegate ^test;
			array<Object^> ^testParams;

		public:
			DT()
			{
				this->notDecisions = gcnew Dictionary<T, IDTNode^>();
				this->decisions = gcnew Dictionary<T, IDTNode^>();
			}

			DT(Delegate ^test) :DT()
			{
				this->test = test;
			}

			DT(Delegate ^test, Object ^testParam) :DT(test)
			{
				this->testParams = gcnew array<Object^> {testParam};
			}

			void addDecision(T val, IDTNode ^n)
			{
				decisions->Add(val, n);
			}

			void addNotDecisions(T val, IDTNode ^n)
			{
				notDecisions->Add(val, n);
			}

			virtual void Execute()
			{
				auto result = (test == nullptr ? nullptr : test->DynamicInvoke(testParams));

				if (result != nullptr)
				{
					auto r = (T)result;

					if (decisions->ContainsKey(r))
						decisions[r]->Execute();
					else
						for each (auto k in notDecisions->Keys)
							if (!r->Equals(k))
								notDecisions[k]->Execute();
				}
			}
		};
	}
}