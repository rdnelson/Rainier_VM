#ifndef __JNE_H__
#define __JNE_H__

#include "Instruction.h"

class Jne : public Instruction {
	friend class Instruction;
protected:
	Jne(char* eip);

public:
	void Execute();
};

#endif
