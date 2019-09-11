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
		ref class State; // cross reference
		ref class SMEnumerable; // cross reference

		public ref class Transaction
		{
		internal:
			State ^toState, ^state;
			System::Collections::IEnumerator ^waiter;

		internal:
			Transaction(State ^state, State ^toState)
			{
				this->state = state;
				this->toState = toState;
			}

			Transaction(State ^state, State ^toState, Func<bool> ^endCondition) :Transaction(state, toState, gcnew UnityEngine::WaitUntil(endCondition)) {}

			Transaction(State ^state, State ^toState, System::Collections::IEnumerator ^waiter) :Transaction(state, toState)
			{
				this->waiter = waiter;
			}
		};

		public ref class State
		{
		internal:
			String ^name;

			UnityEngine::MonoBehaviour ^this_;

			Delegate ^entryAction, ^exitAction;
			array<Object ^> ^parameters;

			HashSet<Transaction ^> ^transactions;

		internal:
			State(String ^name)
			{
				this->name = name;
			}
		};

		private ref class SMEnumerator :System::Collections::IEnumerator
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
				auto state = transaction->state;
				auto next_state = transaction->toState;

				if (state->entryAction != nullptr)
				{
					state->entryAction->DynamicInvoke(state->parameters);
					state->entryAction = nullptr;
				}

				auto more_elements_available = (transaction->waiter != nullptr && transaction->waiter->MoveNext());

				if (!more_elements_available && callback_function != nullptr)
				{
					if (state->exitAction != nullptr)
					{
						state->exitAction->DynamicInvoke(state->parameters);
						state->exitAction = nullptr;
					}

					if (transaction->waiter != nullptr)
					{
						callback_function->DynamicInvoke(next_state);
						callback_function = nullptr;
					}
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

		private ref class SMEnumerable :System::Collections::IEnumerable
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
		internal:
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

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_)
			{
				auto state = gcnew State(name);

				state->transactions = gcnew HashSet<Transaction ^>();
				state->this_ = this_;

				states->Add(state);

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction)
			{
				auto state = addState(name, this_);

				state->entryAction = entryAction;

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction, array<Object ^> ^parameters)
			{
				auto state = addState(name, this_, entryAction);

				state->parameters = parameters;

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, System::Collections::IEnumerator ^waiter, Delegate ^exitAction, State ^toState)
			{
				auto state = addState(name, this_);

				state->exitAction = exitAction;
				state->transactions->Add(gcnew Transaction(state, toState, waiter));

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction, System::Collections::IEnumerator ^waiter, State ^toState)
			{
				auto state = addState(name, this_);

				state->entryAction = entryAction;
				state->transactions->Add(gcnew Transaction(state, toState, waiter));

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, Delegate ^entryAction, Func<bool> ^endCondition, State ^toState)
			{
				auto state = addState(name, this_);

				state->entryAction = entryAction;
				state->transactions->Add(gcnew Transaction(state, toState, endCondition));

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, Func<bool> ^endCondition, Delegate ^exitAction, State ^toState)
			{
				auto state = addState(name, this_);

				state->exitAction = exitAction;
				state->transactions->Add(gcnew Transaction(state, toState, endCondition));

				return state;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^this_, Func<bool> ^endCondition, State ^toState)
			{
				auto state = addState(name, this_);

				state->transactions->Add(gcnew Transaction(state, toState, endCondition));

				return state;
			}
		};
	}
}