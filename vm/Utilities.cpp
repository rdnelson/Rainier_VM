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

#define OP_ARGNUM
#include "common/Opcode.h"

char* Utilities::LoadString(VM* vm, unsigned int address)
{
	int strLength;
	memcpy((char*)&strLength, vm->mData + address, sizeof(strLength));
	char* str = new char[strLength+1];
	memset(str, 0, strLength + 1);
	std::cerr << "Length: " << strLength << std::endl;
	memcpy(str, vm->mData + address + sizeof(strLength), strLength);
	return str;
}

int Utilities::GetArgType(int n, char subcode)
{
	std::cerr << "GetArgType:: subcode: " << (int)subcode << " subcode_n: " << (int)SUBCODE_N(n, subcode) << std::endl;
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
		return TYPE_Def_Address;
	default:
		return TYPE_None;
	}
	return TYPE_None;
}

void Utilities::LoadOpcodeArgs(Opcode * op, char* mText, unsigned int * registers)
{
	std::cerr << "Beginning load, eip = " << registers[REG_EIP] << std::endl;
	switch(op->opcode)
	{
	case NOP_OP:
	case SYSCALL_OP:
	case NOT_OP:
		return;
	default:
		load_byte(op->subcode);
	}

	std::cerr << "Read subcode, eip = " << registers[REG_EIP] << std::endl;

	std::cerr << "Subcode: " << std::hex << (int)op->subcode << std::dec << std::endl;
	if (op->subcode == SUBCODE(SC_REG, SC_REG)) { //split register (only one byte)
		load_byte(op->args[0]);
		op->args[1] = op->args[0] & 0xF;
		op->args[0] >>= 4;
	} else {

		for(int i = 0; i < OP_ArgNum[op->opcode]; i++) {
			std::cerr << "Type of arg" << i << ": " << GetArgType(i, op->subcode) << std::endl;
			op->argtype[i] = GetArgType(i, op->subcode);
			switch(GetArgType(i,op->subcode))
			{
			case TYPE_Constant:
			case TYPE_Address:
				load_arg(op->args[i]);
				break;
			case TYPE_Register:
				load_byte(op->args[i]);
				break;
			case TYPE_Def_Address:
			case TYPE_None:
			default:
				break;
			}
			std::cerr << "Finished reading arg" << i + 1 << " it equals " << op->args[i] << " eip = " << registers[REG_EIP] << std::endl;
		}
	}
}
