#ifndef __OPCODE_H__
#define __OPCODE_H__

#define ADDR_SIZE sizeof(unsigned int)

#define load_reg(arg) arg = 0; arg = (unsigned int)mText[EIP]; EIP++
#define load_id(arg) load_reg(arg)
#define load_argn(arg, size) memset(&arg, 0, sizeof(arg)); memcpy((char*)&arg, &mText[EIP], size); EIP += size
#define load_arg(arg) load_argn(arg, ADDR_SIZE)

enum ArgTypes {
	TYPE_None,
	TYPE_Register,
	TYPE_Address,
	TYPE_Constant,
	TYPE_Id,
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


	SYSCALL_OP = 0xFF
};

struct Opcode {
	unsigned char Opcode;
	unsigned int arg1;
	unsigned char arg1type;
	unsigned int arg2;
	unsigned char arg2type;
	unsigned char isValid;
};

#endif
