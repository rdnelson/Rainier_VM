#include "Not.h"
#include "VM.h"

#include "common/Opcode.h"

Not::Not(char* eip)
{
	mEipOffset += LoadArgs(1, eip);
}

void Not::Execute()
{
	unsigned int val;

	switch(subcode[0])
	{
	case SC_REG:
		val = VM_INSTANCE()->GetRegister(arguments[0]);
		VM_INSTANCE()->SetRegister(arguments[0], ~val);
		break;
	}
}

