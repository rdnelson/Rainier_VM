FILES=$( find . \( -path "*.cpp" -a ! -path "./.*" \)  -printf "%h/%f " )
echo Generating dependency files for:
for f in $FILES
do
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
		echo -n "$i " >> $f.d
	done
	echo >> $f.d
	echo -n "	\$(CC) \$(DEBUG) \$(CFLAGS) -o $OBJFILE $FILE" >> $f.d
	if [ "$(uname -p)" == 'x86_64' ]
	then
		echo "\$(MFLAG32)" >> $f.d
	else
		echo "" >> $f.d
	fi
done
