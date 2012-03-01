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
		if(VM_INSTANCE()->ValidAddress((char*)arguments[0]))
			VM_INSTANCE()->SetRegister(REG_EIP, arguments[0]);
	}
}

