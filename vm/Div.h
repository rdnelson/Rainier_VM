#ifndef __DIV_H__
#define __DIV_H__

#include "Instruction.h"

class Div : public Instruction {
	friend class Instruction;
protected:
	Div(char* eip);

public:
	void Execute();
};

#endif
