#ifndef __OR_H__
#define __OR_H__

#include "Instruction.h"

class Or : public Instruction {
	friend class Instruction;
protected:
	Or(char* eip);

public:
	void Execute();
};

#endif
