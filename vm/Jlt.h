#ifndef __JLT_H__
#define __JLT_H__

#include "Instruction.h"

class Jlt : public Instruction {
	friend class Instruction;
protected:
	Jlt(char* eip);

public:
	void Execute();
};

#endif
