#ifndef __REGISTERS_H__
#define __REGISTERS_H__

enum Registers {
	REG_EAX,
	REG_EBX,
	REG_ECX,
	REG_EDX,
	REG_ESI,
	REG_EDI,
	REG_EBP,
	REG_ESP,
	REG_EIP,
	NUM_REGISTERS
};

static const char *  const STR_Registers[NUM_REGISTERS] = {
	"eax",
	"ebx",
	"ecx",
	"edx",
	"esi",
	"edi",
	"ebp",
	"esp",
	"eip"
};

#endif
