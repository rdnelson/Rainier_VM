#include "Jmp.h"
#include "VM.h"

Jmp::Jmp(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Jmp::Execute()
{
	ResolveValue(0);

	if(Condition()) {
		char* target = VM_INSTANCE()->GetMemory(arguments[0]);
		if(target)
			VM_INSTANCE()->SetRegister(REG_EIP, arguments[0]);
	}
}

