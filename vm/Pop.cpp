#include "Pop.h"
#include "VM.h"

#include "common/Opcode.h"
#include <cstring>

Pop::Pop(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Pop::Execute()
{
	unsigned int tmp;
	int esp;
	char* src;
	switch(subcode[0])
	{
	case SC_REG:
		esp = VM_INSTANCE()->GetRegister(REG_ESP);
		src = VM_INSTANCE()->GetMemory(esp);
		if(src){
			memcpy(&tmp, src, 4);
			VM_INSTANCE()->GetLogger() << "Popping value: 0x" << std::hex << tmp << std::dec << std::endl;
			VM_INSTANCE()->SetRegister(arguments[0], tmp);
			VM_INSTANCE()->SetRegister(REG_ESP, esp + 4);
		}
		break;
	default:
		VM_INSTANCE()->GetLogger() << "Cannot pop to non-register value" << std::endl;
	}

}

