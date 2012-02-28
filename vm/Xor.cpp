#include "Xor.h"
#include "VM.h"

Xor::Xor(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Xor::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

