/*
 * Utilities.cpp
 *
 *  Created on: 2012-01-12
 *      Author: daishi
 */

#include "Utilities.h"
#include "VM.h"
#include <cstring>
#include <iostream>

char* Utilities::LoadString(VM* vm, unsigned int address)
{
	int strLength;
	memcpy((char*)&strLength, vm->mData, sizeof(strLength));
	char* str = new char[strLength+1];
	memset(str, 0, strLength + 1);
	std::cerr << "Length: " << strLength << std::endl;
	memcpy(str, vm->mData + sizeof(strLength), strLength);
	return str;
}

int Utilities::GetArgType(int n, char subcode)
{
	switch(SUBCODE_N(n, subcode))
	{
	case SC_CONST:
	case SC_CONST_P_EAX:
	case SC_CONST_M_EAX:
	case SC_EBX_P_CONST:
	case SC_EBX_M_CONST:
		return TYPE_Constant;
		break;
	case SC_REG:
		return TYPE_Register;
		break;
	case SC_CONST_ADD:
		return TYPE_Address;
	case SC_EBX:
	case SC_EBX_P_EAX:
	case SC_EBX_M_EAX:
		return TYPE_None;
	default:
		return TYPE_None;
	}
	return TYPE_None;
}

void Utilities::LoadOpcodeArgs(Opcode * op, char* mText, unsigned int * registers)
{
	for(int i = 0; i < 2; i++) {
		switch(GetArgType(1,op->subcode))
		{
		case TYPE_Constant:
		case TYPE_Address:
			load_arg(op->arg1);
			break;
		case TYPE_Register:
			load_reg(op->arg1);
			break;
		case TYPE_None:
		default:
			break;
		}
	}

}
