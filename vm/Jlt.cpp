#include "Jlt.h"
#include "VM.h"

Jlt::Jlt(char* eip) : Jmp(eip)
{
	//mEipOffset += LoadArgs(1, eip);
}

bool Jlt::Condition()
{
	return !VM_INSTANCE()->GetFlag(FLAG_GREATER) && !VM_INSTANCE()->GetFlag(FLAG_EQUALS);
}

