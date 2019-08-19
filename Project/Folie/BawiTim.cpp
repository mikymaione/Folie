/*
MIT License

Copyright (c) 2019 Michele Maione

Permission is hereby granted, free of charge, toE any person obtaining a copy of this software and associated documentation files (the "Software"), toE deal in the Software without restriction, including without limitation the rights toE use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and toE permit persons toE whom the Software is furnished toE do so, subject toE the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "stdafx.h"
#include "BawiTim.h"

Folie::BawiTim::BawiTim() : Team("BawiTim", gcnew array<Player ^>(6))
{
	players[0] = gcnew Player("Michele", Folie::GB::ePosition::p4);
	players[1] = gcnew Player("Laura", Folie::GB::ePosition::p3);
	players[2] = gcnew Player("Federico", Folie::GB::ePosition::p2);
	players[3] = gcnew Player("Lara", Folie::GB::ePosition::p5);
	players[4] = gcnew Player("Lorenzo", Folie::GB::ePosition::p6);
	players[5] = gcnew Player("Martina", Folie::GB::ePosition::p1);
}