#ifndef __OPCODE_H__
#define __OPCODE_H__

#define ADDR_SIZE sizeof(unsigned int)

#define load_argn(arg, size) memcpy((char*)&arg, &mText[eip], size); eip += size;
#define load_arg(arg) load_argn(arg, ADDR_SIZE)

enum ArgTypes {
	TYPE_None,
	TYPE_Register,
	TYPE_Address,
	TYPE_Id,
	NUM_TYPES
};

struct Opcode {
	unsigned char Opcode;
	unsigned int arg1;
	unsigned char arg1type;
	unsigned int arg2;
	unsigned char arg2type;
};

#endif
