#include "Sys.h"
#include "VM.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>

Sys::Sys(char* eip)
{
	mEipOffset += 1;
}

void Sys::Execute()
{
	unsigned int eax = VM_INSTANCE()->GetRegister(REG_EAX);
	unsigned int edx = VM_INSTANCE()->GetRegister(REG_EDX);
	unsigned int length;
	char* dst;
	switch(eax)
	{
	case SYS_PRINT_STRING:
		dst = VM_INSTANCE()->GetMemory(edx);
		if(dst) {
			memcpy(&length, dst, 4);
			char * str = new char[length+1];
			str[length] = '\0';
			strncpy(str, dst + 4, length);
			printf("%s", str);
			delete str;
		}
		break;
	case SYS_EXIT:
		exit(VM_INSTANCE()->GetRegister(REG_EDX));
		break;
	}
}


