#ifndef __RET_H__
#define __RET_H__

#include "Instruction.h"

class Ret : public Instruction {
	friend class Instruction;
protected:
	Ret(char* eip);

public:
	void Execute();
};

#endif
