#ifndef __JLE_H__
#define __JLE_H__

#include "Instruction.h"

class Jle : public Instruction {
	friend class Instruction;
protected:
	Jle(char* eip);

public:
	void Execute();
};

#endif
