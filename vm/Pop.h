#ifndef __POP_H__
#define __POP_H__

#include "Instruction.h"

class Pop : public Instruction {
	friend class Instruction;
protected:
	Pop(char* eip);

public:
	void Execute();
};

#endif
