#include "Sub.h"
#include "VM.h"

#include "common/Opcode.h"

Sub::Sub(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Sub::Execute()
{
	ResolveValue(1);
	unsigned int val1;
	unsigned int diff;

	switch(subcode[0]) {
	case SC_REG:
		val1 = VM_INSTANCE()->GetRegister(arguments[0]);
		diff = val1 - arguments[1];
		VM_INSTANCE()->SetRegister(arguments[0], diff);
		if(diff > val1)
			VM_INSTANCE()->SetFlag(FLAG_OFL);
		else
			VM_INSTANCE()->ClearFlag(FLAG_OFL);

		if(diff == 0)
			VM_INSTANCE()->SetFlag(FLAG_ZERO);
		else
			VM_INSTANCE()->ClearFlag(FLAG_ZERO);
		break;

	default:
		VM_INSTANCE()->GetLogger() << "Invalid First Operand For Sub: 0x" << std::hex << subcode[0] << std::dec << std::endl;
	}
}

