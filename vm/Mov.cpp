#include "Mov.h"
#include "VM.h"

#include "common/Opcode.h"

#include <string.h>

Mov::Mov(char* eip) : mCopySize(4)
{
	mEipOffset += LoadArgs(2, eip);
}

void Mov::Execute()
{
	int bitCheck = 0;
	char* dst = 0;

	VM_INSTANCE()->GetLogger() << "Executing Mov: arg1=" << arguments[0] << "arg2=" << arguments[1] << std::endl;

	//produce 0xff in each copyable byte
	for(unsigned int i = 0; i < mCopySize && i < sizeof(int); i++)
		bitCheck = bitCheck << 8 | 0xFF;

	ResolveValue(1); // second argument needs to be resolved to a constant first

	switch(subcode[0]) { //check that first argument is register or address
	case SC_REG:
		//nice and simple
		VM_INSTANCE()->SetRegister(arguments[0], arguments[1] & bitCheck);
		break;

	case SC_CONST_ADD:
	case SC_CONST_P_EAX:
	case SC_CONST_M_EAX:
	case SC_EBX:
	case SC_EBX_P_EAX:
	case SC_EBX_M_EAX:
	case SC_EBX_P_CONST:
	case SC_EBX_M_CONST:
		//ensure it's a valid address before copy
		dst = VM_INSTANCE()->GetMemory(arguments[0]);
		if(dst) {
			//copy the second argument into memory
			arguments[1] &= bitCheck;
			memcpy(dst, &arguments[1], mCopySize);
		} else {
			VM_INSTANCE()->GetLogger() << "Invalid Address: 0x" << std::hex << arguments[0] << " at EIP: 0x" << VM_INSTANCE()->GetRegister(REG_EIP) << std::dec << std::endl;
		}
		break;
	}
}


