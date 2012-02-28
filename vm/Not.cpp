#include "Not.h"
#include "VM.h"

Not::Not(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Not::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

