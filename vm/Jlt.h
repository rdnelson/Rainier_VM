#ifndef __JLT_H__
#define __JLT_H__

#include "Jmp.h"

class Jlt : public Jmp {
protected:
	bool Condition();
public:
	Jlt(char* eip);
};

#endif
