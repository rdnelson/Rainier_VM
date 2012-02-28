#include "Shl.h"
#include "VM.h"

#include "common/Opcode.h"

Shl::Shl(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Shl::Execute()
{
	ResolveValue(1);
	unsigned int val1;
	unsigned int shl;

	switch(subcode[0]) {
	case SC_REG:
		val1 = VM_INSTANCE()->GetRegister(arguments[0]);
		shl = val1 << arguments[1];
		VM_INSTANCE()->SetRegister(arguments[0], shl);

		if(shl == 0)
			VM_INSTANCE()->SetFlag(FLAG_ZERO);
		else
			VM_INSTANCE()->ClearFlag(FLAG_ZERO);
		break;

	default:
		VM_INSTANCE()->GetLogger() << "Invalid First Operand For Shr: 0x" << std::hex << subcode[0] << std::dec << std::endl;
	}
}

