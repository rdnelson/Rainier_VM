#include "Ret.h"
#include "VM.h"

#include <cstring>

Ret::Ret(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Ret::Execute()
{
	unsigned int esp = VM_INSTANCE()->GetRegister(REG_ESP);
	unsigned int retAddr;
	char* src = VM_INSTANCE()->GetMemory(esp);
	if(src) {
		memcpy(&retAddr, src, 4);
		if(VM_INSTANCE()->GetMemory(retAddr))
			VM_INSTANCE()->SetRegister(REG_EIP, retAddr);
		VM_INSTANCE()->SetRegister(REG_ESP, esp + 4);
	}
}

