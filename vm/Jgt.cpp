#include "Jgt.h"
#include "VM.h"

Jgt::Jgt(char* eip) : Jmp(eip)
{
	//mEipOffset += LoadArgs(1, eip);
}

bool Jgt::Condition()
{
	return VM_INSTANCE()->GetFlag(FLAG_GREATER);
}

