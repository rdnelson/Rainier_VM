#!/bin/bash
while [ $# -ne 0 ]; do
	TMP=`mktemp`
	WORKSPACE=`readlink -e $( pwd )`
	if [ -d $1 ]; then
		ABS_DIR=`readlink -e $1`
		REL=`perl -MFile::Spec -e 'print File::Spec->abs2rel("'$WORKSPACE'","'$ABS_DIR'")' `
		#echo $WORKSPACE, $ABS_DIR, $REL
		echo Generating Makefile for $1
		pushd $1 >/dev/null
		SRC=$( find . \( -iname "*.cpp" -o -iname "*.c" \) )
		echo Src: $SRC
		OBJS=
		HPP=
		for f in $SRC; do
			INCFLAG=
			GCCINC=
			for I in $INC; do
				#echo Include Path: $I
				TMPREL=`perl -MFile::Spec -e 'print File::Spec->abs2rel("'$WORKSPACE'","'$( readlink -e $I )'")' `
				INCFLAG="-i $TMPREL/$I $INCFLAG"
				GCCINC="-I$TMPREL/$I $GCCINC"
			done

			INCLUDES=$( $REL/depends.sh $INCFLAG $f )

			INCS=
			for i in $INCLUDES; do
				INCS="`$REL/depends.sh -l $INCFLAG $i` $INCS"
			done
			#echo Includes for $f: $INCS
			OBJ=${f/.cpp/.o}
			OBJ=${OBJ/.c/.o}
			#OBJ=${OBJ/../.../}
			#OBJ=`echo $OBJ | grep -oP '(?<=./)[^/]*'`
			OBJS="$OBJ $OBJS"
			echo >> $TMP
			echo $OBJ: $f $INCS >> $TMP
			echo -e "\t\$(CC) \$(DEBUG) \$(CFLAGS) $GCCINC -o \$@ \$<" >> $TMP
			HPP="`echo $INCS | grep -oP '[^ \t]*.hpp'` $HPP"
			HPP=`echo $HPP | xargs -L 1 -d " " echo | sort -u`
			echo Hpps for $f: $HPP >&2
		done
		HPPOBJ=${HPP//.hpp/.o}
		for s in $HPPOBJ; do
			echo $s: ${s/.o/.cpp} ${s/.o/.hpp} $( $REL/depends.sh $INCFLAG ${s/.o/.hpp} ) >> $TMP
			echo -e "\t\$(CC) \$(DEBUG) \$(CFLAGS) $GCCINC -o \$@ \$<" >> $TMP
		done
		OBJS="$HPPOBJ $OBJS"
		echo include $REL/make.common > Makefile
		echo ".PHONY: all" >> Makefile
		echo "all: "`echo $1 | grep -oP "[^/]*[/]*$" | grep -oP "[^/]*"` >> Makefile
		echo >> Makefile
		echo `echo $1 | grep -oP "[^/]*[/]*$" | grep -oP "[^/]*"`: $( echo $OBJS | xargs -L 1 -d " " echo | sort -u ) >> Makefile
		echo -e "\t\$(CC) \$(DEBUG) \$(LFLAGS) -o \$@ \$^" >> Makefile
		echo -e "\t\$(MKDIR) \$(BINDIR)" >> Makefile
		echo -e "\t\$(CP) \$@ \$(BINDIR)/" >> Makefile
		cat $TMP >> Makefile
		popd >/dev/null
		TARGETS="$1 $TARGETS"
	elif [ "$1" == "-i" ]; then
		#INCFLAG="$INCFLAG $1 $2"
		shift
		INC="$1 $INC"
	elif [ "$1" == "--project" ]; then
		echo "Creating Project Makefile"
		PROJECT="true"
	else
		echo "Invalid Directory:" $1
	fi
	shift
	rm $TMP
done
if [ "$PROJECT" != "" ]; then
	echo include make.common > make.project
	echo ".PHONY: all ${TARGETS//\//} clean" >> make.project
	echo "all: ${TARGETS//\//}" >> make.project
	echo "" >> make.project
	for t in $TARGETS; do
		NAME=`echo $t | grep -oP "[^/]*[/]*$" | grep -oP "[^/]*"`
		echo "${NAME}_make:" >> make.project
		INCFLAG=
		for i in $INC; do
			INCFLAG="-i $i $INCFLAG"
		done
		echo -e "\t \$(GEN_MAKE) $INCFLAG $t" >> make.project
		echo "$NAME:" >> make.project
		echo -e "\t\$(MAKE) --directory=$t all" >> make.project
		echo >> make.project
	done
	echo "clean:" >> make.project
	echo -e "\tfind . -iname \"*.o\" -exec rm {} \\\;" >> make.project
fi

