#include "Ret.h"
#include "VM.h"

Ret::Ret(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Ret::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

