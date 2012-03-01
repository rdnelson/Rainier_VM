#ifndef __JLE_H__
#define __JLE_H__

#include "Jmp.h"

class Jle : public Jmp {
protected:
	bool Condition();
public:
	Jle(char* eip);
};

#endif
