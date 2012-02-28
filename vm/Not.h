#ifndef __NOT_H__
#define __NOT_H__

#include "Instruction.h"

class Not : public Instruction {
	friend class Instruction;
protected:
	Not(char* eip);

public:
	void Execute();
};

#endif
