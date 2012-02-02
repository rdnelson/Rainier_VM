#include "Instruction.h"

#include "SubInstructions.h" //contains includes of all actual instruction subclasses

#define WHITE " 	"

Instruction::Instruction(std::string & line) : mLine(line), mType(-1)
{
}

Instruction* Instruction::CreateInstruction(const std::string & line)
{
	Instruction *newInst;
	int opcodeEnd = line.find_first_of(WHITE); //any whitespace
	std::string opcode = line.substr(0, opcodeEnd);

	for(int i = 0; i < NUM_OPCODES; i++) {
		if(opcode == STR_Opcodes[i]) {
			newInst = CON_Opcodes[i](line);
		}
	}
	return newInst;
}

