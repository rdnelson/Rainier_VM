#include "Je.h"
#include "VM.h"

Je::Je(char* eip) : Jmp(eip)
{
	//mEipOffset += LoadArgs(1, eip);
}

bool Je::Condition()
{
	return VM_INSTANCE()->GetFlag(FLAG_EQUALS);
}
