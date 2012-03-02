#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

class Instruction {
public:
	static Instruction* ReadInstruction(unsigned int);

	virtual bool IsValid() { return mValid; } //if instruction is capable of being run
	virtual int GetError() { return mErr; } //if not valid, return specific error

	virtual void Execute() = 0;

	virtual unsigned int GetEipOffset() { return mEipOffset; }

protected:
	Instruction();

	unsigned int mEipOffset;

	bool mValid;
	int mErr;

	unsigned int LoadArgs(unsigned int argNum, char* eip);
	unsigned int LoadOneArg(unsigned int argNum, char argCode, char* eip);

	void ResolveValue(unsigned int arg);

	unsigned int arguments[2];
	char subcode[2];

};

#endif
