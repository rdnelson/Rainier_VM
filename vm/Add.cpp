#include "Add.h"
#include "VM.h"

#include "common/Opcode.h"

Add::Add(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Add::Execute()
{
	ResolveValue(1);
	unsigned int val1;
	unsigned int sum;

	switch(subcode[0]) {
	case SC_REG:
		//val1 = 
		//sum = 
		break;
	}
}

