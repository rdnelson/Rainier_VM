#include "Test.h"
#include "VM.h"

Test::Test(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Test::Execute()
{
	VM_INSTANCE()->GetMemSize();
}

