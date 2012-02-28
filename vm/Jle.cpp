#include "Jle.h"
#include "VM.h"

Jle::Jle(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jle::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

