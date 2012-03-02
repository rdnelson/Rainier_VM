#include "Argument.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "common/Opcode.h"
#include "common/Registers.h"

#define WHITE " 	"

Argument::Argument() : mType(SC_NONE), mInternalType(0), mVal(0), mText(""), mValid(true)
{
}

Argument::Argument(std::string & arg) : mType(SC_NONE), mInternalType(0), mVal(0), mText(arg), mValid(true)
{
	Init();
}

Argument::~Argument()
{
	for(int i = 0; i < mSubArguments.size(); ++i)
		delete mSubArguments[i];
}
void Argument::Init(std::string & arg)
{
	mText = arg;
	Init();
}

void Argument::Init()
{
	for(int i = 0; i < mSubArguments.size(); ++i)
		delete mSubArguments[i];
	mSubArguments.clear();

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
		mText = mText.substr(1, mText.size() - 2); // cut off brackets
		ParseAddress();
		return;
	}

	//check if the argument is a label
	if(mText.size() >= 2) {
		if(mText[0] == ':'){
			mType = SC_CONST;
			mInternalType = ARG_LABEL;
			mText = mText.substr(1);
			return;
		}else if(mText[0] == '@'){
			mType = SC_CONST;
			mInternalType = ARG_DATA;
			mText = mText.substr(1);
			return;
		}else if(mText[0] == '"') {
			mType = SC_NONE;
			mInternalType = ARG_STRING;

			if(mText[mText.size() - 1 ] == '"') {	
				mText = mText.substr(1, mText.size() - 2); // cut off quotes
				int replace = mText.find("\\n");
				while(replace != mText.npos) {
					mText = mText.replace(replace, 2, "\n");
					replace = mText.find("\\n");
				}
				return;
			}
		}
	}

	if (mText.size() == 1) {
		if(mText[0] == '+' ) {
			mType = SC_NONE;
			mInternalType = ARG_PLUS;
			return;
		} else if (mText[0] == '-') {
			mType = SC_NONE;
			mInternalType = ARG_MINUS;
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

	//it appears to be nothing else, make it a label declaration
	//it's only valid if it's associated with the right command
	mType = SC_NONE;
	mInternalType = ARG_LABEL_DEC;

}

void Argument::ParseAddress()
{
	int tokenBegin = 0, tokenEnd = 0;
	std::string token("");

 	while(tokenEnd != std::string::npos) {
		tokenBegin = mText.find_first_not_of(WHITE, tokenEnd); //skip initial whitespace
		tokenEnd = mText.find_first_of(WHITE, tokenBegin);

		if(tokenBegin == std::string::npos)
			break;

		if(tokenEnd == std::string::npos)
			token = mText.substr(tokenBegin);
		else
			token = mText.substr(tokenBegin, tokenEnd - tokenBegin);

		mSubArguments.push_back(new Argument(token));
	}

	if(mSubArguments.size() == 1) { //possibilities are SC_CONST_ADD, SC_EBX
		switch(mSubArguments[0]->GetType()) {
		case SC_CONST:
			mType = SC_CONST_ADD;
			if(mSubArguments[0]->GetInternalType() == ARG_DATA) {

			} else {
				mVal = mSubArguments[0]->GetVal();
			}
			break;
		case SC_REG:
			if(mSubArguments[0]->GetVal() == REG_EBX) {
				mType = SC_EBX;
				mVal = 0;
				break;
			} else {
				mValid = false; //ebx is the only valid register
			}
			break;
		default:
			mValid = false;
			break;
		}
	} else if(mSubArguments.size() == 3) { //Operator and two operands
		if(mSubArguments[1]->GetInternalType() == ARG_PLUS || mSubArguments[1]->GetInternalType() == ARG_MINUS) {
			switch(mSubArguments[0]->GetType()) {
			case SC_CONST:
				//the only two valid choices are const_p_eax and const_m_eax
				if(mSubArguments[2]->GetType() == SC_REG && mSubArguments[2]->GetVal() == REG_EAX) {
					mType = (mSubArguments[1]->GetInternalType() == ARG_PLUS) ? SC_CONST_P_EAX : SC_CONST_M_EAX;
					mVal = mSubArguments[0]->GetVal();
					break;
				} else {
					mValid = false;
				}
				break;
			case SC_REG:
				//the valid choices are ebx_pm_eax and ebx_pm_const
				if(mSubArguments[0]->GetType() == SC_REG && mSubArguments[0]->GetVal() == REG_EBX) {
					if(mSubArguments[2]->GetType() == SC_REG && mSubArguments[2]->GetVal() == REG_EAX) {
						mType = (mSubArguments[1]->GetInternalType() == ARG_PLUS) ? SC_EBX_P_EAX : SC_EBX_M_EAX;
						mVal = 0;
						break;
					} else if(mSubArguments[2]->GetType() == SC_CONST) {
						mType = (mSubArguments[1]->GetInternalType() == ARG_PLUS) ? SC_EBX_P_CONST : SC_EBX_M_CONST;
						mVal = mSubArguments[2]->GetVal();
						break;
					} else {
						mValid = false;
					}
				} else {
					mValid = false;
				}
			}

		} else {
			mValid = false;
		}
	}
}

bool Argument::HasConstant()
{
	switch(mType) {
	case SC_CONST:
	case SC_CONST_ADD:
	case SC_CONST_P_EAX:
	case SC_CONST_M_EAX:
	case SC_EBX_P_CONST:
	case SC_EBX_M_CONST:
		return true;
		break;
	}

	return false;
}

bool Argument::HasData()
{
	if(mType == SC_NONE) {
		if(mInternalType == ARG_STRING) {
			return true;
		}
	}
	return false;
}

void Argument::Dump()
{
	std::cerr << "Type: " << mType << "	Internal Type: " << mInternalType << std::endl;
	std::cerr << "Value: " << mVal << "	Text: `" << mText << "`" << std::endl;
}

std::string Argument::ToBinary()
{
	std::string retval("");

	if(HasConstant()) {
		char val[4];
		if(mType == SC_CONST_ADD) {
			int tmpVal = mSubArguments[0]->GetVal();
			memcpy(val, &tmpVal, 4);
		} else {
			memcpy(val, &mVal, 4);
		}

		retval.append(val, 4);
	}

	if(mType == SC_REG) {
		retval.push_back((char)mVal);
	}

	return retval;
}

bool Argument::NeedsLabel()
{
	if (mType == SC_CONST && (mInternalType == ARG_LABEL || mInternalType == ARG_DATA))
		return true;

	for(int i = 0; i < mSubArguments.size(); i++)
		if(mSubArguments[i]->NeedsLabel())
			return true;

	return false;
}

void Argument::SubstituteLabels(std::map<std::string, unsigned int> &labelMap)
{
	if (mType == SC_CONST && (mInternalType == ARG_LABEL || mInternalType == ARG_DATA)) {
		if( labelMap.find(mText) != labelMap.end() ) {
			mVal = labelMap[mText];
			mInternalType = ARG_NONE;
		} else {
			mValid = false;
		}
	} else {
		for(int i = 0; i < mSubArguments.size(); i++) {
			if(mSubArguments[i]->NeedsLabel()) { //depth should only be one, but here goes...
				mSubArguments[i]->SubstituteLabels(labelMap);
			}
		}
	}
}

