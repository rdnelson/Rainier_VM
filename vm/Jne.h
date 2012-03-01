#ifndef __JNE_H__
#define __JNE_H__

#include "Jmp.h"

class Jne : public Jmp {
protected:
	bool Condition();
public:
	Jne(char* eip);
};

#endif
