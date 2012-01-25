VM:=vm/
COMPILER:=compiler/
ASSEMBLER:=assembler/

.SILENT: all

.PHONY: all _all $(VM) $(COMPILER) $(ASSEMBLER) clean VM Compiler Assembler

all:
	./gen_makefiles.sh
	$(MAKE) _all

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

clean:
	find . \( -iname "*.o" -o -iname "*.d" \) -exec rm {} \;
