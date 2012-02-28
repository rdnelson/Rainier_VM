#ifndef __LOOP_H__
#define __LOOP_H__

#include "Instruction.h"

class Loop : public Instruction {
	friend class Instruction;
protected:
	Loop(char* eip);

public:
	void Execute();
};

#endif
