#ifndef __JGE_H__
#define __JGE_H__

#include "Instruction.h"

class Jge : public Instruction {
	friend class Instruction;
protected:
	Jge(char* eip);

public:
	void Execute();
};

#endif
