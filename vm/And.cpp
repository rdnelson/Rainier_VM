#include "And.h"
#include "VM.h"

#include "common/Opcode.h"

And::And(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void And::Execute()
{
	ResolveValue(1);

	unsigned int target;

	switch(subcode[0])
	{
	case SC_REG:
		target = VM_INSTANCE()->GetRegister(arguments[0]);
		target &= arguments[1];
		VM_INSTANCE()->SetRegister(arguments[0], target);
		break;
	}
}

