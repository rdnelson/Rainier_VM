#include "Options.h"
#include "VM.h"

int main(int argc, char* argv[])
{
	Options opts(argc, argv);
	if(!opts.IsValid())
		return 1;

	VM vm(&opts);
	vm.run();
}
