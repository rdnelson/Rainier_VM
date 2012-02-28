#ifndef __SUB_H__
#define __SUB_H__

#include "Instruction.h"

class Sub : public Instruction {
	friend class Instruction;
protected:
	Sub(char* eip);

public:
	void Execute();
};

#endif
