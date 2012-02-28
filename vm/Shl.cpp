#include "Shl.h"
#include "VM.h"

Shl::Shl(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Shl::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

