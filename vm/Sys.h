#ifndef __SYS_H__
#define __SYS_H__

#include "Instruction.h"

class Sys : public Instruction {
	friend class Instruction;
protected:
	Sys(char* eip);

public:
	void Execute();
};

#endif
