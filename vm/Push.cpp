#include "Push.h"
#include "VM.h"

#include <cstring>

Push::Push(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Push::Execute()
{
	ResolveValue(0);

	unsigned int esp = VM_INSTANCE()->GetRegister(REG_ESP);
	memcpy(&VM_INSTANCE()->Memory[esp], &arguments[0], 4);
	esp -= 4;
	VM_INSTANCE()->SetRegister(REG_ESP, esp);
}

