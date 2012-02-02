#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <string>
#include "Argument.h"

class Instruction
{
public:
	static Instruction* CreateInstruction(const std::string & line);

	virtual int GetBinaryLen();
	virtual std::string & ToBinary();

	inline int GetType() { return mType; }
	inline std::string& GetLine() { return mLine; }

private:
	Instruction(std::string & line);
	virtual ~Instruction();

	Argument mArgument[2];

	std::string mLine;
	int mType;
}

#endif
