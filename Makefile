VM:=vm/
COMPILER:=compiler/

.SILENT: all

.PHONY: all _all $(VM) $(COMPILER) clean VM Compiler

all:
	./gen_makefiles.sh
	$(MAKE) _all

_all: $(VM) $(COMPILER)

VM:
	./gen_makefiles.sh
	$(MAKE) --directory=$(VM) all

Compiler:
	./gen_makefiles.sh
	$(MAKE) --directory=$(COMPILER) all

$(VM) $(COMPILER):
	$(MAKE) --directory=$@ all

clean:
	find . \( -iname "*.o" -o -iname "*.d" \) -exec rm {} \;
