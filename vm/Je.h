#ifndef __JE_H__
#define __JE_H__

#include "Jmp.h"

class Je : public Jmp {

protected:
	bool Condition();
public:
	Je(char* eip);
};

#endif
