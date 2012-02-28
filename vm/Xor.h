#ifndef __XOR_H__
#define __XOR_H__

#include "Instruction.h"

class Xor : public Instruction {
	friend class Instruction;
protected:
	Xor(char* eip);

public:
	void Execute();
};

#endif
