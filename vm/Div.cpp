#include "Div.h"
#include "VM.h"

Div::Div(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Div::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

