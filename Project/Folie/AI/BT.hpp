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
		interface class IBT
		{
			void Execute();
		};

		generic <class T> ref class BT :IBT
		{
		public:
			LinkedList<IBT^> ^sequentialEndActions;
			Dictionary<T, IBT^> ^chanches, ^negateChanches;

			Delegate ^test;
			array<Object^> ^testParams;

		public:
			BT()
			{
				this->negateChanches = gcnew Dictionary<T, IBT^>();
				this->chanches = gcnew Dictionary<T, IBT^>();

				this->sequentialEndActions = gcnew LinkedList<IBT^>();
			}

			BT(Delegate ^test) :BT()
			{
				this->test = test;
			}

			BT(Delegate ^test, Object ^testParam) :BT(test)
			{
				this->testParams = gcnew array<Object^> {testParam};
			}

			virtual void Execute()
			{
				auto result = (test == nullptr ? nullptr : test->DynamicInvoke(testParams));

				if (result != nullptr)
				{
					auto r = (T)result;

					if (chanches->ContainsKey(r))
						chanches[r]->Execute();
					else
						for each (auto k in negateChanches->Keys)
							if (!r->Equals(k))
								negateChanches[k]->Execute();
				}

				for each (auto decision in sequentialEndActions)
					decision->Execute();
			}
		};
	}
}