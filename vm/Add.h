#ifndef __ADD_H__
#define __ADD_H__

#include "Instruction.h"

class Add : public Instruction {
	friend class Instruction;
protected:
	Add(char* eip);

public:
	void Execute();
};

#endif
