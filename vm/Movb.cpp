#include "Movb.h"
#include "VM.h"

Movb::Movb(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Movb::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

