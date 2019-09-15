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

		public ref class Transition
		{
		internal:
			State ^toState, ^fromState;

			Func<bool> ^condition;

			System::Collections::IEnumerator ^waiter;

		internal:
			Transition(State ^fromState, State ^toState)
			{
				this->fromState = fromState;
				this->toState = toState;
			}

			Transition(State ^fromState, Func<bool> ^condition, State ^toState) :Transition(fromState, toState)
			{
				this->condition = condition;
			}

			Transition(State ^fromState, State ^toState, System::Collections::IEnumerator ^waiter) :Transition(fromState, toState)
			{
				this->waiter = waiter;
			}

			Transition(State ^fromState, State ^toState, Func<bool> ^endCondition) :Transition(fromState, toState, gcnew UnityEngine::WaitUntil(endCondition)) {}
		};

		public ref class State
		{
		internal:
			String ^name;

			UnityEngine::MonoBehaviour ^monoBehaviour;

			Delegate ^enterAction, ^exitAction;
			array<Object ^> ^enterParameters, ^exitParameters;

			HashSet<Transition ^> ^transitions;

		internal:
			State(String ^name)
			{
				this->name = name;
			}
		};

		private ref class SMEnumerator sealed :System::Collections::IEnumerator
		{
		private:
			Transition ^transition;
			Delegate ^callback_function;

		public:
			SMEnumerator(Transition ^transition, Delegate ^callback_function)
			{
				this->transition = transition;
				this->callback_function = callback_function;
			}

			virtual bool MoveNext()
			{
				auto fromState = transition->fromState;

				auto more_elements_available = transition->waiter->MoveNext();

				if (!more_elements_available)
				{
					if (fromState->exitAction != nullptr)
						fromState->exitAction->DynamicInvoke(fromState->exitParameters);

					auto next_state = transition->toState;

					if (callback_function != nullptr)
						callback_function->DynamicInvoke(next_state, fromState);
				}

				return more_elements_available;
			}

			virtual void Reset()
			{
				transition->waiter->Reset();
			}

			virtual property Object ^Current
			{
				Object ^get()
				{
					return transition->waiter->Current;
				}
			}
		};

		private ref class SMEnumerable sealed :System::Collections::IEnumerable
		{
		private:
			SMEnumerator ^smEnumerator;

		public:
			SMEnumerable(Transition ^transition, Delegate ^callback_function)
			{
				this->smEnumerator = gcnew SMEnumerator(transition, callback_function);
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

			void run(State ^to_)
			{
				if (current == nullptr)
					run(to_, nullptr); // for circular reference bug
				else
					run(to_, current);
			}

			void run(State ^to_, State ^from_)
			{
				auto transactionToThisStateIsPossible = (from_ == nullptr);

				if (!transactionToThisStateIsPossible)
					for each (auto t in from_->transitions)
						if (t->toState == to_)
						{
							transactionToThisStateIsPossible = true;
							break;
						}

				if (transactionToThisStateIsPossible)
				{
					current = to_;

					if (current->enterAction != nullptr)
						current->enterAction->DynamicInvoke(current->enterParameters);

					for each (auto t in current->transitions)
					{
						if (t->waiter == nullptr)
						{
							if (t->condition != nullptr)
								if (t->condition())
									run(t->toState);
						}
						else
						{
							auto ge = gcnew SMEnumerable(
								t,
								gcnew Action<State^, State^>(this, &SM::run)
							);

							current->monoBehaviour->StartCoroutine(ge->GetEnumerator());
						}
					}
				}
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour)
			{
				auto fromState = gcnew State(name);

				fromState->transitions = gcnew HashSet<Transition ^>();
				fromState->monoBehaviour = monoBehaviour;

				states->Add(fromState);

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, Delegate ^enterAction)
			{
				auto fromState = addState(name, monoBehaviour);

				fromState->enterAction = enterAction;

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, Delegate ^enterAction, array<Object ^> ^enterParameters)
			{
				auto fromState = addState(name, monoBehaviour, enterAction);

				fromState->enterParameters = enterParameters;

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, System::Collections::IEnumerator ^waiter, Delegate ^exitAction, State ^toState)
			{
				auto fromState = addState(name, monoBehaviour);

				fromState->exitAction = exitAction;
				fromState->transitions->Add(gcnew Transition(fromState, toState, waiter));

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, Delegate ^enterAction, System::Collections::IEnumerator ^waiter, State ^toState)
			{
				auto fromState = addState(name, monoBehaviour);

				fromState->enterAction = enterAction;
				fromState->transitions->Add(gcnew Transition(fromState, toState, waiter));

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, Delegate ^enterAction, Func<bool> ^endCondition, State ^toState)
			{
				auto fromState = addState(name, monoBehaviour);

				fromState->enterAction = enterAction;
				fromState->transitions->Add(gcnew Transition(fromState, toState, endCondition));

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, Func<bool> ^endCondition, Delegate ^exitAction, State ^toState)
			{
				auto fromState = addState(name, monoBehaviour);

				fromState->exitAction = exitAction;
				fromState->transitions->Add(gcnew Transition(fromState, toState, endCondition));

				return fromState;
			}

			State ^addState(String ^name, UnityEngine::MonoBehaviour ^monoBehaviour, Func<bool> ^endCondition, State ^toState)
			{
				auto fromState = addState(name, monoBehaviour);

				fromState->transitions->Add(gcnew Transition(fromState, toState, endCondition));

				return fromState;
			}
		};
	}
}