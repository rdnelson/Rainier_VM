#include "Sys.h"
#include "VM.h"

Sys::Sys(char* eip)
{
	mEipOffset += 1;
}

void Sys::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

