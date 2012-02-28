#ifndef __PUSH_H__
#define __PUSH_H__

#include "Instruction.h"

class Push : public Instruction {
	friend class Instruction;
protected:
	Push(char* eip);

public:
	void Execute();
};

#endif
