#include "Movb.h"
#include "VM.h"

Movb::Movb(char* eip) : Mov(eip)
{
	mCopySize = 1;
	//mEipOffset += LoadArgs(2, eip);
}

