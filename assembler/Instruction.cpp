#include "Instruction.h"

#include <cstring>

#define OP_STRINGS
#define OP_BUILDERS
#include "common/Opcode.h"

#define WHITE " 	"

Instruction::Instruction(const std::string & line) : mLine(line), mType(-1)
{
}

Instruction::~Instruction()
{
	for(int i = 0; i < mArguments.size(); ++i)
		delete mArguments[i];
}

//assume everything is in lowercase
Instruction* Instruction::CreateInstruction(const std::string & line)
{
	Instruction *newInst = new Instruction(line);
	std::string opcode;

	int opcodeBegin = 0;
	int opcodeEnd = 0;
	opcodeBegin = line.find_first_not_of(WHITE, opcodeEnd); //skip initial whitespace
	opcodeEnd = line.find_first_of(WHITE, opcodeBegin);

	//only whitespace, so no instruction
	if(opcodeBegin == std::string::npos)
		return 0;

	//if there's no trailing whitespace, take the whole string
	if(opcodeEnd == std::string::npos)
		opcode = line.substr(opcodeBegin);
	else
		opcode = line.substr(opcodeBegin, opcodeEnd - opcodeBegin);

	if (opcode[0] == '#')
		return 0;

	for(int i = 0; i < NUM_OPCODES; i++) {
		if(opcode == STR_Opcodes[i]) {
			newInst->mType = i;
			return newInst;
		}
	}

	//The few extra assembler only commands
	if(opcode == "ds") {
		newInst->mType = DS_OP;
		return newInst;
	} else if (opcode[0] == ':') { //label
		newInst->mType = LABEL_OP;
		return newInst;
	}

	return 0;
}

void Instruction::ParseArguments()
{
	int tokenBegin = 0, tokenEnd = 0;
	std::string token("");

	tokenBegin = mLine.find_first_not_of(WHITE, tokenEnd); //skip initial whitespace
	tokenEnd = mLine.find_first_of(WHITE, tokenBegin); //read in opcode, to ignore it.

 	while(tokenEnd != std::string::npos) {
		tokenBegin = mLine.find_first_not_of(WHITE, tokenEnd); //skip initial whitespace
		tokenEnd = mLine.find_first_of(WHITE, tokenBegin);

		if(tokenBegin == std::string::npos)
			break;

		if(tokenEnd == std::string::npos)
			token = mLine.substr(tokenBegin);
		else
			token = mLine.substr(tokenBegin, tokenEnd - tokenBegin);

		if(token[0] == '#')
			break;

		if(token[0] == '[') {
			tokenEnd = mLine.find_first_of("]", tokenBegin) + 1;
			token = mLine.substr(tokenBegin, tokenEnd - tokenBegin);
		}

		mArguments.push_back(new Argument(token));
	}
}

std::string Instruction::ToBinary()
{
	if(!IsValid())
		return "";

	std::string tmpBinary("");
	if(mType >= 0 ) { //actual code command
		tmpBinary.push_back((char)mType); // output actual command code

		if(mArguments.size() == 1) {
			tmpBinary.push_back(SUBCODE(mArguments[0]->GetType(),SC_NONE));
			tmpBinary.append(mArguments[0]->ToBinary());
		} else if (mArguments.size() == 2) {
			tmpBinary.push_back(SUBCODE(mArguments[0]->GetType(), mArguments[1]->GetType()));
			if(mArguments[0]->GetType() == SC_REG && mArguments[1]->GetType() == SC_REG) {
				//handle split register
				tmpBinary.push_back(SUBCODE(mArguments[0]->GetVal(), mArguments[1]->GetVal()));
			} else {
				tmpBinary.append(mArguments[0]->ToBinary());
				tmpBinary.append(mArguments[1]->ToBinary());
			}
		}
	} else if(mType == DS_OP) {
		char size[4];
		int iSize = mArguments[1]->GetText().size();
		memcpy(size, &iSize, 4);
		tmpBinary.append(size, 4);
		tmpBinary.append(mArguments[1]->GetText().c_str(), mArguments[1]->GetText().size());
	}

	return tmpBinary;
}

int Instruction::GetBinaryLen()
{
	return ToBinary().size();
}

bool Instruction::IsValid()
{

	return true;
}

bool Instruction::IsLabelDef()
{
	if(!IsValid())
		return false;

	if(mType == LABEL_OP)
		return true;

	for(int i = 0; i < mArguments.size(); i++) {
		if(mType == DS_OP) {
			if(mArguments[i]->GetInternalType() == Argument::ARG_LABEL_DEC) {
				return mArguments.size() == 2;
			}
		}
	}
	return false;
}

bool Instruction::NeedsLabel()
{

	for(int i = 0; i < mArguments.size(); i++) {
		if (mArguments[i]->GetInternalType() == Argument::ARG_LABEL || mArguments[i]->GetInternalType() == Argument::ARG_DATA) {
			return true;
		}
	}
	return false;
}

std::string Instruction::GetLabelDefName()
{
	if(!IsLabelDef())
		return "";

	if(mType == LABEL_OP)
		return mLine;

	return mArguments[1]->GetText();
}

void Instruction::SubstituteLabels(std::map<std::string, unsigned int> &labelMap)
{
	for(int i = 0; i < mArguments.size(); i++) {
		if(mArguments[i]->GetInternalType() == Argument::ARG_LABEL || mArguments[i]->GetInternalType() == Argument::ARG_DATA) {
			if(labelMap.find(mArguments[i]->GetText()) != labelMap.end()) {
				mArguments[i]->SetVal(labelMap[mArguments[i]->GetText()]);
			}
		}
	}
}

bool Instruction::IsText()
{
	if(mType < 0)
		return true && IsValid();
}
