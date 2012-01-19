/*
 * Utilities.cpp
 *
 *  Created on: 2012-01-12
 *      Author: daishi
 */

#include "Utilities.h"
#include "VM.h"
#include <cstring>

char* Utilities::LoadString(VM* vm, unsigned int address)
{
	int strLength;
	memcpy((char*)&strLength, vm->mData, sizeof(strLength));
	char* str = new char[strLength+1];
	memset(str, 0, strLength + 1);
	memcpy(str, vm->mData + sizeof(strLength), strLength);
	return str;
}
