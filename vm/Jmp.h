#ifndef __JMP_H__
#define __JMP_H__

#include "Instruction.h"

class Jmp : public Instruction {
	friend class Instruction;
protected:
	Jmp(char* eip);

public:
	void Execute();
};

#endif
