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
	VM_INSTANCE()->GetLogger() << "Pushing value: " << arguments[0] << std::endl;
	unsigned int esp = VM_INSTANCE()->GetRegister(REG_ESP);
	esp -= 4;
	char* dst = VM_INSTANCE()->GetMemory(esp);
	if(dst) {
		memcpy(dst, &arguments[0], 4);
		VM_INSTANCE()->SetRegister(REG_ESP, esp);
	}
}

