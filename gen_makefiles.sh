#!/bin/bash
./depends.sh
PROJECTS=$( find . \( -type d -a ! -path "./.*" -a ! -path "./*/*" -a ! -path "." \) )
for DIR in $PROJECTS
do
	cd $DIR
	echo "include ../make.common" > makefile
	echo "OBJS=" >> makefile
	echo .PHONY: all >> makefile
	echo "include \$(shell find . -iname \"*cpp.d\" -printf \"%h/%f \" )" >> makefile
	#echo "\$(warning OBJS: \$(OBJS))" >> makefile
	echo $( echo $DIR | tail -c +3 ): "\$(OBJS)" >> makefile 
	echo "	\$(CC) \$(DEBUG) \$(LFLAGS) -o \$@ \$^" >> makefile
	echo "	\$(MKDIR) \$(BINDIR)" >> makefile
	echo "	\$(CP) \$@ \$(BINDIR)/\$@" >> makefile
	echo >> makefile
	echo "all: $( echo $DIR | tail -c +3 )" >> makefile
	cd ..
done
