#ifndef __MOV_H__
#define __MOV_H__

#include "Instruction.h"

class Mov : public Instruction {
	friend class Instruction;
protected:
	Mov(char* eip);

public:
	void Execute();
};

#endif
