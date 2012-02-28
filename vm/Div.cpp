#include "Div.h"
#include "VM.h"

#include "common/Opcode.h"

Div::Div(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Div::Execute()
{
	ResolveValue(1);
	unsigned int val1;
	unsigned int quot;

	if(arguments[1] == 0) {
		VM_INSTANCE()->GetLogger() << "Divide by zero attempted" << std::endl;
		return;
	}

	switch(subcode[0]) {
	case SC_REG:
		val1 = VM_INSTANCE()->GetRegister(arguments[0]);
		quot = val1 / arguments[1];
		VM_INSTANCE()->SetRegister(arguments[0], quot);
		if(quot > val1)
			VM_INSTANCE()->SetFlag(FLAG_OFL);
		else
			VM_INSTANCE()->ClearFlag(FLAG_OFL);

		if(quot == 0)
			VM_INSTANCE()->SetFlag(FLAG_ZERO);
		else
			VM_INSTANCE()->ClearFlag(FLAG_ZERO);
		break;

	default:
		VM_INSTANCE()->GetLogger() << "Invalid First Operand For Add: 0x" << std::hex << subcode[0] << std::dec << std::endl;
	}
}

