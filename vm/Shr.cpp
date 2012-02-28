#include "Shr.h"
#include "VM.h"

Shr::Shr(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Shr::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

