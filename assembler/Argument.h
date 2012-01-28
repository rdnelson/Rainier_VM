#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <string>

class Argument
{
public:
	Argument();
	Argument(std::string & arg);

	void Init();
	void Init(std::string &arg);

	inline int GetType() { return mType; }
	inline int GetVal() { return mVal; }

	bool IsData(); // value stored in val does not need resolution
	bool IsRegister(); // value stored needs resolution
	bool IsAddress(); //value stored in val is an address and might need resolution
	bool IsSubcodeAddress(); //value stored needs registers to resolve

	std::string & ToBinary();

	enum Internal_Types {
		ARG_NONE = 0,
		ARG_VM = 0,
		ARG_LABEL, //for jump points
		ARG_DATA, //for data addresses
		ARG_STRING, //for string constants
	};

	void Dump();

private:

	void ParseAddress() {}
	int mType;
	int mInternalType;
	int mVal;

	std::string mText;
};

#endif
