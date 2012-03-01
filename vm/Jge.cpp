#include "Jge.h"
#include "VM.h"

Jge::Jge(char* eip) : Jmp(eip)
{
	//mEipOffset += LoadArgs(1, eip);
}

bool Jge::Condition()
{
	return VM_INSTANCE()->GetFlag(FLAG_EQUALS) || VM_INSTANCE()->GetFlag(FLAG_GREATER);
}

