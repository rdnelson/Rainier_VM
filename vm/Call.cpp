#include "Call.h"
#include "VM.h"

Call::Call(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Call::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

