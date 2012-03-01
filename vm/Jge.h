#ifndef __JGE_H__
#define __JGE_H__

#include "Jmp.h"

class Jge : public Jmp {
protected:
	bool Condition();
public:
	Jge(char* eip);
};

#endif
