#ifndef __JGT_H__
#define __JGT_H__

#include "Instruction.h"

class Jgt : public Instruction {
	friend class Instruction;
protected:
	Jgt(char* eip);

public:
	void Execute();
};

#endif
