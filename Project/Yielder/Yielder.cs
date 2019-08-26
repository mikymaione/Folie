/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
using System;
using System.Collections;
using UnityEngine;

namespace Yielder
{
    public static class Yielder
    {

        public static void waiter<T>(MonoBehaviour this_, ushort seconds, Action<T> ok, T p1)
        {
            this_.StartCoroutine(coroutine_waiter(seconds, ok, p1));
        }

        private static IEnumerator coroutine_waiter<T>(ushort seconds, Action<T> ok, T p1)
        {
            yield return new WaitForSeconds(seconds);
            ok(p1);
        }

        public static void waiter(MonoBehaviour this_, ushort seconds, Action ok)
        {
            this_.StartCoroutine(coroutine_waiter(seconds, ok));
        }

        private static IEnumerator coroutine_waiter(ushort seconds, Action ok)
        {
            yield return new WaitForSeconds(seconds);
            ok();
        }

        public static void waiterFor(MonoBehaviour this_, Func<bool> evalutation, Action ok)
        {
            this_.StartCoroutine(coroutine_waiterFor(evalutation, ok));
        }

        private static IEnumerator coroutine_waiterFor(Func<bool> evalutation, Action ok)
        {
            yield return new WaitUntil(evalutation);
            ok();
        }


    }
}