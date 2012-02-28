#include "Jge.h"
#include "VM.h"

Jge::Jge(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jge::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

