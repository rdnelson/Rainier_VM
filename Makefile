VM:=vm/
COMPILER:=compiler/
ASSEMBLER:=assembler/
UNITTESTS:=unittests/
include make.common

.SILENT: all

.PHONY: all _all $(VM) $(COMPILER) $(ASSEMBLER) $(UNITTESTS) clean VM Compiler Assembler Unit_tests cppcheck gtest

all:
	./gen_makefiles.sh
	$(MAKE) $(UNITTESTS)

_all: $(VM) $(COMPILER) $(ASSEMBLER)

VM:
	./gen_makefiles.sh
	$(MAKE) --directory=$(VM) all

Compiler:
	./gen_makefiles.sh
	$(MAKE) --directory=$(COMPILER) all

Assembler:
	./gen_makefiles.sh
	$(MAKE) --directory=$(ASSEMBLER) all

$(VM) $(COMPILER) $(ASSEMBLER):
	$(MAKE) --directory=$@ all

$(UNITTESTS): gtest _all
	./gen_makefiles.sh
	$(MAKE) --directory=$@ all LFLAGS="$(LFLAGS) $(GTEST_LFLAGS)" CFLAGS="$(CFLAGS) -I../"

$(LIBDIR)/lib$(GTEST_LIB).a:
	$(MKDIR) $(LIBDIR)/build
	$(CC) -I$(GTEST_SRC)/include -I$(GTEST_SRC) -c $(GTEST_SRC)/src/gtest-all.cc -o $(LIBDIR)/build/gtest-all.o
	$(CC) -I$(GTEST_SRC)/include -I$(GTEST_SRC) -c $(GTEST_SRC)/src/gtest_main.cc -o $(LIBDIR)/build/gtest_main.o
	$(AR) rv $(LIBDIR)/lib$(GTEST_LIB).a $(LIBDIR)/build/gtest-all.o $(LIBDIR)/build/gtest_main.o

clean:
	find . \( -iname "*.o" -o -iname "*.d" \) -exec rm {} \;

cppcheck:
	$(CPPCHECK) --enable=all --xml 2>cppcheck-results.xml -I include

gtest: $(LIBDIR)/lib$(GTEST_LIB).a

Unit_tests: $(UNITTESTS)

