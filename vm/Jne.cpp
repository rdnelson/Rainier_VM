#include "Jne.h"
#include "VM.h"

Jne::Jne(char* eip) : Jmp(eip)
{
	//mEipOffset += LoadArgs(1, eip);
}

bool Jne::Condition()
{
	return !VM_INSTANCE()->GetFlag(FLAG_EQUALS);
}

