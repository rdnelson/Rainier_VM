#ifndef __TEST_H__
#define __TEST_H__

#include "Instruction.h"

class Test : public Instruction {
	friend class Instruction;
protected:
	Test(char* eip);

public:
	void Execute();
};

#endif
