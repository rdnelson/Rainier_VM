#ifndef __MUL_H__
#define __MUL_H__

#include "Instruction.h"

class Mul : public Instruction {
	friend class Instruction;
protected:
	Mul(char* eip);

public:
	void Execute();
};

#endif
