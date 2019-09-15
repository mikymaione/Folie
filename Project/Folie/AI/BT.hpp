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
		interface class IBTNode
		{
			bool Execute();
		};


		ref class BTQuestion :IBTNode
		{
		public:
			Delegate ^boolFN;
			array<Object^> ^params;

		public:
			BTQuestion(Delegate ^boolFN)
			{
				this->boolFN = boolFN;
			}

			virtual bool Execute()
			{
				return (bool)boolFN->DynamicInvoke(params);
			}
		};

		generic <class T> ref class BTAction:IBTNode
		{
		public:
			Delegate ^action;
			array<T> ^params;

		public:
			BTAction(Delegate ^action)
			{
				this->action = action;
			}

			BTAction(Delegate ^action, T param):BTAction(action)
			{				
				this->params = gcnew array<T>{param};
			}

			virtual bool Execute()
			{
				action->DynamicInvoke(params);

				return true;
			}
		};

		generic <class T> ref class BTInverter :BTAction<T>
		{
		public:
			virtual bool Execute() override
			{
				return !BTAction::Execute();
			}
		};


		ref class BTComposite abstract :IBTNode
		{
		public:
			LinkedList<IBTNode^> ^childrens;
		public:
			virtual bool Execute() abstract;
		};

		ref class BTSelector :BTComposite
		{
		public:
			virtual bool Execute() override
			{
				for each (auto c in childrens)
					if (c->Execute())
						return true;

				return false;
			}
		};

		ref class BTSequence :BTComposite
		{
		public:
			virtual bool Execute() override
			{
				for each (auto c in childrens)
					if (!c->Execute())
						return false;

				return true;
			}
		};


		ref class BT :IBTNode
		{
		public:
			IBTNode ^node;

		public:
			BT(IBTNode ^node)
			{
				this->node = node;
			}

			virtual bool Execute()
			{
				node->Execute();
			}			
		};
	}
}