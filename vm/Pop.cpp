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
	switch(subcode[0])
	{
	case SC_REG:
		esp = VM_INSTANCE()->GetRegister(REG_ESP);
		memcpy(&tmp, &VM_INSTANCE()->Memory[esp], 4);
		VM_INSTANCE()->SetRegister(arguments[0], tmp);
		break;
	default:
		VM_INSTANCE()->GetLogger() << "Cannot pop to non-register value" << std::endl;
	}

}

