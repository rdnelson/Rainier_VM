#include "Mul.h"
#include "VM.h"

#include "common/Opcode.h"

Mul::Mul(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Mul::Execute()
{
	ResolveValue(1);
	unsigned int val1;
	unsigned int prod;

	switch(subcode[0]) {
	case SC_REG:
		val1 = VM_INSTANCE()->GetRegister(arguments[0]);
		prod = val1 * arguments[1];
		VM_INSTANCE()->SetRegister(arguments[0], prod);
		if(prod < val1)
			VM_INSTANCE()->SetFlag(FLAG_OFL);
		else
			VM_INSTANCE()->ClearFlag(FLAG_OFL);

		if(prod == 0)
			VM_INSTANCE()->SetFlag(FLAG_ZERO);
		else
			VM_INSTANCE()->ClearFlag(FLAG_ZERO);
		break;

	default:
		VM_INSTANCE()->GetLogger() << "Invalid First Operand For Mul: 0x" << std::hex << subcode[0] << std::dec << std::endl;
	}
}

