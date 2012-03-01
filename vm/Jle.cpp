#include "Jle.h"
#include "VM.h"

Jle::Jle(char* eip) : Jmp(eip)
{
	//mEipOffset += LoadArgs(1, eip);
}

bool Jle::Condition()
{
	return (!VM_INSTANCE()->GetFlag(FLAG_GREATER) && !VM_INSTANCE()->GetFlag(FLAG_EQUALS)) || VM_INSTANCE()->GetFlag(FLAG_EQUALS);
}

