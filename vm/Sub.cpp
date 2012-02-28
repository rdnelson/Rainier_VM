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
		val1 = VM_INSTANCE()->GetRegister(argument[0]);
		diff = val1 - argument[1];
		VM_INSTANCE()->SetRegister(argument[0], diff);
		if(diff > val1)
			VM_INSTANCE()->SetFlag(CPU_OFL);
		else
			VM_INSTANCE()->ClearFlag(CPU_OFL);

		if(diff == 0)
			VM_INSTANCE()->SetFlag(CPU_ZERO);
		else
			VM_INSTANCE()->ClearFlag(CPU_ZERO);
		break;

	default:
		VM_INSTANCE()->GetLogger() << "Invalid First Operand For Sub: 0x" << std::hex << subcode[0] << std::dec << std::endl;
	}
}

