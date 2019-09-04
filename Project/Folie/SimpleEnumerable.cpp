#include "stdafx.h"
#include "SimpleEnumerable.hpp"

bool Folie::SimpleEnumerable::SimpleEnumerator::MoveNext()
{
	return false;
}

void Folie::SimpleEnumerable::SimpleEnumerator::Reset()
{
	throw gcnew System::NotImplementedException();
}
