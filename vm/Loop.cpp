#include "Loop.h"
#include "VM.h"

#include "Jmp.h"

Loop::Loop(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Loop::Execute()
{
	unsigned int ecx = VM_INSTANCE()->GetRegister(REG_ECX);
	if(ecx) {
		ResolveValue(0);
		if(VM_INSTANCE()->GetMemory(arguments[0]))
			VM_INSTANCE()->SetRegister(REG_EIP, arguments[0]);
		VM_INSTANCE()->SetRegister(REG_ECX, ecx - 1);

	}
}

