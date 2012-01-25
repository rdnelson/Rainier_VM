COMMON=include
FILES=$( find . \( -path "*.cpp" -a ! -path "./.*" \)  -printf "%h/%f " )
echo Generating dependency files for:
for f in $FILES
do
	if [ $f -ot $f.d ]
	then
		continue
	fi
	OBJFILE=${f/.cpp/.o}
	OBJFILE=${OBJFILE/.c/.o}
	OBJFILE=$( echo $OBJFILE | sed -n 's|./[a-zA-Z0-9]*/||p' )
	FILE=$( echo $f | sed -n 's|./[a-zA-Z0-9]*/||p' )
	echo $f
	echo "OBJS+= $OBJFILE" > $f.d
	echo -n "$OBJFILE: $FILE " >> $f.d
	INCLUDES=$( grep -h "#include" $f | sed -n 's/#include[ tab]*"//p' | sed -n 's/"//p' | tr "\\n" " " )
	for i in $INCLUDES
	do
		if [ ! -e $i -a -e $COMMON/$i ]
		then
			echo -n "\$(COMMON)/$i " >> $f.d
		else
			echo -n "$i " >> $f.d
		fi
	done
	echo >> $f.d
	echo -n "	\$(CC) \$(DEBUG) \$(CFLAGS) -o $OBJFILE $FILE " >> $f.d
	if [ "$(uname -p)" == 'i386' ]
	then
		echo "\$(MFLAG32)" >> $f.d
	elif [ "$(uname -p)" == 'x86_64' ]
	then
		echo "\$(MFLAG64)" >> $f.d
	elif [ "$(uname -p)" == 'armv71' ]
	then
		echo "\$(MFLAGARM)" >> $f.d
	else
		echo "" >> $f.d
	fi
done
