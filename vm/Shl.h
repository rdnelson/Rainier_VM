#ifndef __SHL_H__
#define __SHL_H__

#include "Instruction.h"

class Shl : public Instruction {
	friend class Instruction;
protected:
	Shl(char* eip);

public:
	void Execute();
};

#endif
