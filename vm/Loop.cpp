#include "Loop.h"
#include "VM.h"

Loop::Loop(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Loop::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

