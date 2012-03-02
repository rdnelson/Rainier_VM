#include "Call.h"
#include "VM.h"

#include <cstring>

Call::Call(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Call::Execute()
{
	ResolveValue(0);
	unsigned int esp = VM_INSTANCE()->GetRegister(REG_ESP);
	unsigned int eip = VM_INSTANCE()->GetRegister(REG_EIP);
	char* dst = VM_INSTANCE()->GetMemory(esp - 4);
	if(dst) {
		memcpy(dst, &eip, 4);
		VM_INSTANCE()->SetRegister(REG_ESP, esp -4);
		if(VM_INSTANCE()->GetMemory(arguments[0]))
			VM_INSTANCE()->SetRegister(REG_EIP, arguments[0]);
	}
}

