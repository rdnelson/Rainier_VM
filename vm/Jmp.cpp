#include "Jmp.h"
#include "VM.h"

Jmp::Jmp(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jmp::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

