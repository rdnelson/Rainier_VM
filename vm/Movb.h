#ifndef __MOVB_H__
#define __MOVB_H__

#include "Instruction.h"

class Movb : public Instruction {
	friend class Instruction;
protected:
	Movb(char* eip);

public:
	void Execute();
};

#endif
