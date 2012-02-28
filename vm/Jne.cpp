#include "Jne.h"
#include "VM.h"

Jne::Jne(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jne::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

