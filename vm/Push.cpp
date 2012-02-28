#include "Push.h"
#include "VM.h"

Push::Push(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Push::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

