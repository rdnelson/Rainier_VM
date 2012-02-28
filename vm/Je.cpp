#include "Je.h"
#include "VM.h"

Je::Je(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Je::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

