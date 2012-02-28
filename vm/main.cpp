#include "Options.h"
#include "VM.h"

int main(int argc, char* argv[])
{
	Options opts(argc, argv);

	VM_INSTANCE()->SetOptions(&opts);
	VM_INSTANCE()->Start();
#ifdef WIN32
	system("pause");
#endif
}
