#include "Jlt.h"
#include "VM.h"

Jlt::Jlt(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jlt::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

