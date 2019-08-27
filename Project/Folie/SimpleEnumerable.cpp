#include "stdafx.h"
#include "SimpleEnumerable.h"

bool Folie::SimpleEnumerable::SimpleEnumerator::MoveNext()
{
	return false;
}

void Folie::SimpleEnumerable::SimpleEnumerator::Reset()
{
	throw gcnew System::NotImplementedException();
}
