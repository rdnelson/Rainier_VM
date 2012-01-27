#ifndef __OPCODE_H__
#define __OPCODE_H__

#include <iostream>

#define ADDR_SIZE sizeof(unsigned int)

#define load_byte(arg) arg = 0; arg = mText[EIP]; EIP++
#define load_reg(arg) arg = 0; arg = (unsigned int)mText[EIP]; EIP++
#define load_id(arg) load_reg(arg)
#define load_argn(arg, size) memset(&arg, 0, sizeof(arg)); memcpy((char*)&arg, &mText[EIP], size); EIP += size
#define load_arg(arg) load_argn(arg, ADDR_SIZE)

enum ArgTypes {
	TYPE_None,
	TYPE_Register,
	TYPE_Address,
	TYPE_Def_Address, //for VM
	TYPE_Constant,
	TYPE_Id,
	TYPE_Operator, //for assembler
	NUM_TYPES
};

enum Opcodes {
	NOP_OP,
	MOV_OP,
	ADD_OP,
	SUB_OP,
	MUL_OP,
	DIV_OP,
	SHR_OP,
	SHL_OP,
	PUSH_OP,
	POP_OP,
	JMP_OP,
	TEST_OP,
	AND_OP,
	OR_OP,
	XOR_OP,
	NOT_OP,
	LOOP_OP,
	JE_OP,
	JNE_OP,
	JGT_OP,
	JGE_OP,
	JLT_OP,
	JLE_OP,
	SYSCALL_OP
};

static char OP_ArgNum[] = {
	0,
	2,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	2,
	1,
	1,
	1,
	0,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	0
};

enum Subcode {
	SC_NONE,
	SC_CONST,
	SC_REG,
	SC_CONST_ADD,
	SC_EBX,
	SC_EBX_P_EAX,
	SC_EBX_M_EAX,
	SC_CONST_P_EAX,
	SC_CONST_M_EAX,
	SC_EBX_P_CONST,
	SC_EBX_M_CONST, //9
};

#define SUBCODE(dst,src) (char)(((dst << 4) & 0xF0) | (src & 0x0F))
#define SUBCODE1(code) ((code >> 4) & 0xF)
#define SUBCODE2(code) (code & 0xF)
#define SUBCODE_N(n, code) ((char)((code >> (4 * (n == 0 ? 1 : 0))) & 0xF))

struct Opcode {
	unsigned char opcode;
	unsigned char subcode;
	unsigned char isValid;
	unsigned int args[2];
	unsigned char argtype[2];

	void printop()
	{
		std::cerr << "Opcode: " << std::hex << (int)opcode << "\nSubcode: " << (int)subcode << "\nIsValid: " << (int)isValid << std::dec << std::endl;
		for(int i = 0; i < 2; i++)
			std::cerr << "Arg" << i << std::hex << ": 0x" << args[i] << std::dec << "	Type: " << (int)argtype[i] << std::endl;
	}
};

#endif
