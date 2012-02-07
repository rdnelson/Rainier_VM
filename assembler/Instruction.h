#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <string>
#include <vector>
#include <map>

#include "Argument.h"

class Instruction
{
public:

	~Instruction();

	static Instruction* CreateInstruction(const std::string & line);

	int GetBinaryLen();
	std::string ToBinary();

	inline int GetType() { return mType; }
	inline std::string GetLine() { return mLine; }

	void ParseArguments();

	bool IsLabelDef();
	bool IsDataDef();
	bool NeedsLabel();

	std::string GetLabelDefName();

	void SubstituteLabels(std::map<std::string, unsigned int> &labelMap);

	bool IsValid();
	bool IsText();

private:

	Instruction(const std::string & line);

	std::vector<Argument*> mArguments;

	std::string mLine;
	int mType;
};

#endif
