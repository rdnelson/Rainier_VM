#include "Argument.h"

#include <cctype>
#include <cstdlib>
#include <iostream>

#include "common/Opcode.h"
#include "common/Registers.h"

Argument::Argument() : mType(SC_NONE), mInternalType(0), mVal(0), mText("")
{
}

Argument::Argument(std::string & arg) : mType(SC_NONE), mInternalType(0), mVal(0), mText(arg)
{
	Init();
}
void Argument::Init(std::string & arg)
{
	mText = arg;
	Init();
}

void Argument::Init()
{
	//first parse it into standard form (lcase, no whitespace)

	int i = 0;
	for(i = 0; i < mText.size(); ++i) {
		mText[i] = std::tolower(mText[i]);
	}

	if (mText == "")
		return;

	//check if the argument is a register
	for(i = 0; i < NUM_REGISTERS; i++) {
		if (mText == STR_Registers[i]) {
			mVal = i;
			mType = SC_REG;
			return;
		}
	}

	//check if the argument is an address
	if (mText[0] == '[' && mText[mText.size() - 1] == ']') {
		ParseAddress();
		return;
	}

	//check if the argument is a label
	if(mText.size() >= 2) {
		if(mText[0] == ':'){
			mType = SC_NONE;
			mInternalType = ARG_LABEL;
			mText = mText.substr(1);
			return;
		}else if(mText[0] == '@'){
			mType = SC_NONE;
			mInternalType = ARG_DATA;
			mText = mText.substr(1);
			return;
		}else if(mText[0] == '"' && mText[mText.size() - 1 ] == '"') {
			mType = SC_NONE;
			mInternalType = ARG_STRING;
			mText = mText.substr(1, mText.size() - 2); // cut off quotes
			return;
		}
	}

	//check if the argument is a hex constant
	if(mText.size() >= 3) {
		if(mText[0] == '0' && mText[1] == 'x') {
			//for(i = 2; i < mText.size(); ++i) {
			//	if( //TODO: finish
			mType = SC_CONST;
			mVal = 0xDEAFBEEF;
			return;
		}
	}

	//check if the argument is a constant
	bool valid = true;
	for(i = 0; i < mText.size(); ++i) {
		if(!std::isdigit(mText[i])) {
			valid = false;
			break;
		}
	}
	if (valid) {
		mType = SC_CONST;
		mVal = atoi(mText.c_str());
		return;
	}
}

bool Argument::IsData()
{
	if(mType == SC_CONST)
		return true;

	return false;
}

bool Argument::IsRegister()
{
	return false;
}

void Argument::Dump()
{
	std::cerr << "Type: " << mType << "	Internal Type: " << mInternalType << std::endl;
	std::cerr << "Value: " << mVal << "	Text: `" << mText << "`" << std::endl;
}
