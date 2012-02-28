#include "Or.h"
#include "VM.h"

Or::Or(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Or::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

