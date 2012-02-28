#ifndef __AND_H__
#define __AND_H__

#include "Instruction.h"

class And : public Instruction {
	friend class Instruction;
protected:
	And(char* eip);

public:
	void Execute();
};

#endif
