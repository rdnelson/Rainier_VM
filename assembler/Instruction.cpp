#include "Instruction.h"

#define OP_STRINGS
#define OP_BUILDERS
#include "common/Opcode.h"

#define WHITE " 	"

Instruction::Instruction(const std::string & line) : mLine(line), mType(-1)
{
}

Instruction::~Instruction()
{
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
	}

	return 0;
}

void Instruction::ParseArguments()
{
	int tokenBegin = 0, tokenEnd = 0;
	std::string token("");

	

}
