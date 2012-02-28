#include "Mul.h"
#include "VM.h"

Mul::Mul(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Mul::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

