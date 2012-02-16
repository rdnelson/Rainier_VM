#!/bin/bash
./depends.sh
PROJECTS=$( find . \( -type d -a ! -path "./.*" -a ! -path "./*/*" -a ! -path "." \) )
for DIR in $PROJECTS
do
	cd $DIR
	NUMFILES=$( find . -maxdepth 1 \( -type "f" -a ! -path "." -a -path "./*.cpp" -a ! -path "./.*" \) | wc -l )
	echo "include ../make.common" > makefile
	echo "OBJS=" >> makefile
	echo "EXTRAOBJ=" >> makefile
	echo .PHONY: all >> makefile
	echo "include \$(shell find . -iname \"*cpp.d\" -printf \"%h/%f \" )" >> makefile
	#echo "\$(warning OBJS: \$(OBJS))" >> makefile
	echo $( echo $DIR | tail -c +3 ): "\$(OBJS)" >> makefile
	if [ $NUMFILES != 0 ]
	then
		echo "	\$(CC) \$(DEBUG) \$(LFLAGS) -o \$@ \$^ \$(EXTRAOBJ)" >> makefile
		echo "	\$(MKDIR) \$(BINDIR)" >> makefile
		echo "	\$(CP) \$@ \$(BINDIR)/\$@" >> makefile
	fi
	echo >> makefile
	echo "all: $( echo $DIR | tail -c +3 )" >> makefile
	cd ..
done
