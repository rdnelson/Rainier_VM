#include "Pop.h"
#include "VM.h"

Pop::Pop(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Pop::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

