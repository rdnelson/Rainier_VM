#include "Sub.h"
#include "VM.h"

Sub::Sub(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Sub::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

