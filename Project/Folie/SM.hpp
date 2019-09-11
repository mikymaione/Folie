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
	ref class State; // cross reference
	ref class SMEnumerable; // cross reference

	public ref class Transaction
	{
	internal:
		State ^toState;
		System::Collections::IEnumerator ^waiter;

	public:
		Transaction(State ^toState, Func<bool> ^endCondition) :Transaction(toState, gcnew UnityEngine::WaitUntil(endCondition)) {}

		Transaction(State ^toState, System::Collections::IEnumerator ^waiter)
		{
			this->toState = toState;
			this->waiter = waiter;
		}
	};

	public ref class State
	{
	internal:
		UnityEngine::MonoBehaviour ^this_;

	public:
		Delegate ^entryAction, ^exitAction;
		array<Object ^> ^parameters;

		HashSet<Transaction ^> ^transactions;

	public:
		State(UnityEngine::MonoBehaviour ^this_)
		{
			this->transactions = gcnew HashSet<Transaction ^>();
			this->this_ = this_;
		}

		State(UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction) :State(this_)
		{
			this->entryAction = entryAction;
		}

		State(UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction, array<Object ^> ^parameters) :State(this_, entryAction)
		{
			this->parameters = parameters;
		}

		State(UnityEngine::MonoBehaviour ^this_, System::Collections::IEnumerator ^waiter, Delegate ^exitAction, State ^toState) :State(this_)
		{
			this->exitAction = exitAction;
			this->transactions->Add(gcnew Transaction(toState, waiter));
		}

		State(UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction, System::Collections::IEnumerator ^waiter, State ^toState) :State(this_)
		{
			this->entryAction = entryAction;
			this->transactions->Add(gcnew Transaction(toState, waiter));
		}

		State(UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction, Func<bool> ^endCondition, State ^toState) :State(this_)
		{
			this->entryAction = entryAction;
			this->transactions->Add(gcnew Transaction(toState, endCondition));
		}

		State(UnityEngine::MonoBehaviour ^this_, Func<bool> ^endCondition, State ^toState) :State(this_)
		{
			this->transactions->Add(gcnew Transaction(toState, endCondition));
		}
	};

	ref class SMEnumerator :System::Collections::IEnumerator
	{
	private:
		Transaction ^transaction;
		Delegate ^callback_function;

	public:
		SMEnumerator(Transaction ^transaction, Delegate ^callback_function)
		{
			this->transaction = transaction;
			this->callback_function = callback_function;
		}

		virtual bool MoveNext()
		{
			auto state = transaction->toState;

			if (state->entryAction != nullptr)
			{
				state->entryAction->DynamicInvoke(state->parameters);
				state->entryAction = nullptr;
			}

			auto more_elements_available = transaction->waiter->MoveNext();

			if (!more_elements_available && callback_function != nullptr)
			{
				if (state->exitAction != nullptr)
				{
					state->exitAction->DynamicInvoke(state->parameters);
					state->exitAction = nullptr;
				}

				callback_function->DynamicInvoke(state);
				callback_function = nullptr;
			}

			return more_elements_available;
		}

		virtual void Reset()
		{
			transaction->waiter->Reset();
		}

		virtual property Object ^Current
		{
			Object ^get()
			{
				return transaction->waiter->Current;
			}
		}
	};

	ref class SMEnumerable :System::Collections::IEnumerable
	{
	private:
		SMEnumerator ^smEnumerator;

	public:
		SMEnumerable(Transaction ^transaction, Delegate ^callback_function)
		{
			this->smEnumerator = gcnew SMEnumerator(transaction, callback_function);
		}

		virtual System::Collections::IEnumerator ^GetEnumerator()
		{
			return smEnumerator;
		}
	};

	public ref class SM
	{
	public:
		State ^current;
		HashSet<State ^> ^states;

	public:
		SM()
		{
			states = gcnew HashSet<State ^>();
		}

		void run(State ^current)
		{
			this->current = current;

			for each (auto t in current->transactions)
			{
				auto ge = gcnew SMEnumerable(
					t,
					gcnew Action<State ^>(this, &SM::run)
				);

				current->this_->StartCoroutine(ge->GetEnumerator());
			}
		}
	};
}