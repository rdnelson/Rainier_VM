#include "And.h"
#include "VM.h"

And::And(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void And::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

