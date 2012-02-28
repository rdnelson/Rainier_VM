#ifndef __NOP_H__
#define __NOP_H__

#include "Instruction.h"

class Nop : public Instruction {
	friend class Instruction;
protected:
	Nop(char* eip);

public:
	void Execute();
	bool IsValid();
	int GetError();

};

#endif
