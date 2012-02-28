#include "Jgt.h"
#include "VM.h"

Jgt::Jgt(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jgt::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

