#include "Test.h"
#include "VM.h"

Test::Test(char* eip)
{
	mEipOffset += LoadArgs(2, eip);
}

void Test::Execute()
{
	ResolveValue(0);
	ResolveValue(1);

	if(arguments[0] == arguments[1])
		VM_INSTANCE()->SetFlag(FLAG_EQUALS);
	else
		VM_INSTANCE()->ClearFlag(FLAG_EQUALS);

	if(arguments[0] > arguments[1])
		VM_INSTANCE()->SetFlag(FLAG_GREATER);
	else
		VM_INSTANCE()->ClearFlag(FLAG_GREATER);

}

