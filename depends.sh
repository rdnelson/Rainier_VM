#!/bin/bash

while [ $# -gt 0 ]; do
	#file exists or there are include directories to check
	if [ "$1" == "-i" ]; then
		shift
		INC="$1 $INC"
	elif [ "$1" == "-l" ]; then
		LOC="1"
	elif [  -f $1 -o "$INC" != "" ]; then
		if [ "$TMP" == "" ]; then
			TMP=`mktemp`
		fi

		if [ -f $1 ]; then
			FIL=$1
		else
			#check include directories
			FIL=`find $INC -path "*$1" 2>/dev/null`
			if [ "$DEBUG" != "" ]; then
				echo Finding \"*$1\" in path: \"$INC\" >&2
				echo Found: $FIL >&2
			fi
		fi

		#if file doesn't exist
		if [ "$FIL" == "" ]; then
			echo Included file $1 not found. >&2
			shift
			continue;
		fi
		if [ "$LOC" == "" ]; then
			#if [ $FIL -ot $FIL.o ]; then
			#	continue
			#fi
			echo $FIL >> $TMP

			if [ "$DEBUG" != "" ]; then
				echo Finding Dependencies for $FIL >&2
			fi

			#find all of the include files referenced in the current file
			FILES=$( grep -ozP '^[ \t]*#include[ \t]*\"[^\"]*\"' $FIL | xargs -0 echo | grep -oP '(?<=\")[^\"]*(?=\")' )

			#create marker file

			if [ "$DEBUG" != "" ]; then
				echo The Included Files for $FIL are: $FILES >&2
			fi
			#recursively check the include files
			for f in $FILES; do

				if [ "$DEBUG" != "" ]; then
					echo Checking: $f >&2
				fi

				INCFLAG=
				#parse the includes for recursion
				for I in $INC; do
					INCFLAG="-i $I $INCFLAG"
				done

				if [ "`grep \"$f\" $TMP`" == "" ]; then

					if [ "$INCFLAG" != "" ]; then
						SUB="$( ./$0 --temp $TMP $DEBUG $INCFLAG $f ) $SUB"
					else
						SUB="$( ./$0 --temp $TMP $DEBUG $f ) $SUB"
					fi
				fi
			done

			#combine and condense primary and recursive dependencies
			DEPS=$( echo $FILES $SUB | xargs -L 1 -d " " echo | sort -u )
			if [ "$DEBUG" != "" ]; then
				echo Dependencies for $FIL are: $DEPS >&2
			fi
			#final return for this file
			echo $DEPS
		else
			echo $FIL
		fi
	elif [ "$1" == "-d" ]; then
		DEBUG="-d"
		echo Setting Debug Flag >&2
	elif [ "$1" == "--temp" ]; then
		shift
		if [ "$TMP" == "" ]; then
			TMP=$1
		else
			echo "temp must be set before any files are passed" >&2
		fi
	else
		echo "Invalid Argument: " $1 >&2
	fi
	shift
done
