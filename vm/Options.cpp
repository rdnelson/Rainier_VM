/*
 * Options.cpp
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#include "Options.h"
#include <string>
#include <stdio.h>
#include <cstring>
#include <cstdlib>

Options::Options(int argc, char* argv[]) : mValid(false), mVerbose(false), mStep(false), mMemSize(MEM_SIZE), mPageSize(PAGE_SIZE) {
	// TODO Auto-generated constructor stub
	if( argc == 1)
	{
		mValid = false;
		return;
	}

	for(int i = 1; i < argc - 1; ++i) {

		if(!strcmp(argv[i], "-v")) {
			mVerbose = true;
		} else if (!strcmp(argv[i], "--step")) {
			mStep = true;
		} else if (!strcmp(argv[i], "-m")) {
			mMemSize = atoi(argv[++i]);
		}
	}

	mExe = argv[argc - 1];


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

