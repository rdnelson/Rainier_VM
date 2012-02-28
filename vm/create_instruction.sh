#!/bin/bash

#create files
cp Mov.cpp $1.cpp
cp Mov.h $1.h

#update them with correct name
echo "#include \"$1.h\"" >> Instructions.h
echo $1 | xargs -I xxx sed -i "s/Mov/xxx/gp" xxx.{cpp,h}
echo $1 | xargs -I xxx sed -i "s/MOV/\Uxxx\E/gp" xxx.{cpp,h}
uniq $1.cpp $1.tmp
mv $1.tmp $1.cpp
uniq $1.h $1.tmp
mv $1.tmp $1.h

