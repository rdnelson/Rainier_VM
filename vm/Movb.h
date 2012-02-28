#ifndef __MOVB_H__
#define __MOVB_H__

#include "Instruction.h"
#include "Mov.h"

class Movb : public Mov {

public:
	Movb(char* eip);
protected:
	//unsigned int mCopySize;
};

#endif
