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
		ref class DT
		{
		public:
			LinkedList<DT^> ^decisions;
			Dictionary<Object^, DT^> ^chanches, ^negateChanches;

			Delegate ^Test;
			array<Object^> ^testParams;

		public:
			DT()
			{
				this->negateChanches = gcnew Dictionary<Object^, DT^>();
				this->chanches = gcnew Dictionary<Object^, DT^>();

				this->decisions = gcnew LinkedList<DT^>();
			}

			DT(Delegate ^Test) :DT()
			{
				this->Test = Test;
			}

			DT(Delegate ^Test, array<Object^> ^testParams) :DT(Test)
			{
				this->testParams = testParams;
			}

			DT(Delegate ^Test, Object ^testParam) :DT(Test)
			{
				this->testParams = gcnew array<Object^> {testParam};
			}

			void Execute()
			{
				auto result = (Test == nullptr ? nullptr : Test->DynamicInvoke(testParams));

				if (result != nullptr)
				{
					if (chanches->ContainsKey(result))
						chanches[result]->Execute();
					else
						for each (auto k in negateChanches->Keys)
							if (!result->Equals(k))
								negateChanches[k]->Execute();
				}

				for each (auto decision in decisions)
					decision->Execute();
			}
		};
	}
}