#ifndef __JGT_H__
#define __JGT_H__

#include "Jmp.h"

class Jgt : public Jmp {
protected:
	bool Condition();
public:
	Jgt(char* eip);
};

#endif
