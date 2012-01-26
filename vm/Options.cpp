/*
 * Options.cpp
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#include "Options.h"
#include <string>
#include <stdio.h>

Options::Options(int argc, char* argv[]) {
	// TODO Auto-generated constructor stub
	if( argc == 1)
	{
		mValid = false;
		return;
	}

	int exeIndex = 1;
	if(!strcmp(argv[exeIndex], "-v")) {
		mVerbose = true;
		exeIndex++;
	}
	else
		mVerbose = false;

	mExe = argv[exeIndex];


	FILE* tmpFile = fopen(mExe.c_str(), "r");
	if(!tmpFile)
	{
		mValid = false;
		return;
	}

	fclose(tmpFile);
	mValid = true;
}

Options::~Options() {
	// TODO Auto-generated destructor stub
}

