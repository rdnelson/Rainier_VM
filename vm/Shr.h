#ifndef __SHR_H__
#define __SHR_H__

#include "Instruction.h"

class Shr : public Instruction {
	friend class Instruction;
protected:
	Shr(char* eip);

public:
	void Execute();
};

#endif
