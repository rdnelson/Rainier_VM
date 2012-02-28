#include "Nop.h"


Nop::Nop(char* eip)
{
}

void Nop::Execute()
{
	//nop doesn't do anything
}

bool Nop::IsValid()
{
	return true; //nothing can go wrong if we're at this point
}

int Nop::GetError()
{
	return 0; //there are never any errors
}
