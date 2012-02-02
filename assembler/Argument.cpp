#include "Argument.h"

#include <cctype>
#include "common/Opcode.h"

Argument::Argument(std::string & arg) : mType(SC_NONE), mVal(0), mText(arg)
{
	//first parse it into standard form (lcase, no whitespace)

	int i = 0;
	for(i = 0; i < mText.size(); ++i) {
		mText[i] = std::tolower(mText[i]);
	}

	if (mText == "")
		return

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
		if(mText[0] == ':' || mText[0] == '@'){
			mType = -SC_CONST;
			return;
		}
	}
	
	//check if the argument is a hex constant
	if(mText.size() >= 3) {
		if(mText[0] == '0' && mText[1] == 'x') {
			for(i = 2; i < mText.size(); ++i) {
				if( //TODO: finish

	//check if the argument is a constant
	bool valid = true;
	for(i = 0; i < mText.size(); ++i) {
		if(!std::isdigit(mText[i])
}

bool Argument::IsData()
{
	if(mType == SC_CONST || mType == SC_ADD)
		return true;

	return false;
}

bool Argument::IsRegister()
{
	if(mType
