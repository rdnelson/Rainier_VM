#ifndef __CALL_H__
#define __CALL_H__

#include "Instruction.h"

class Call : public Instruction {
	friend class Instruction;
protected:
	Call(char* eip);

public:
	void Execute();
};

#endif
