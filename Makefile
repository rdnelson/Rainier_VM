include make.common

.PHONY: makefiles

include make.project

makefiles:
	$(GEN_MAKE) -i include assembler/ compiler/ vm/ -i assembler unittests/

make.project:
	$(GEN_MAKE) --project -i include assembler/ compiler/ vm/ -i assembler unittests/
