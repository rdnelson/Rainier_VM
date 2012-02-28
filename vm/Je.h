#ifndef __JE_H__
#define __JE_H__

#include "Instruction.h"

class Je : public Instruction {
	friend class Instruction;
protected:
	Je(char* eip);

public:
	void Execute();
};

#endif
