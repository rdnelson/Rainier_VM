#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

#include <string>
#include <vector>

class Argument
{
public:
	Argument();
	Argument(std::string & arg);

	~Argument();

	void Init();
	void Init(std::string &arg);

	inline int GetType() { return mType; }
	inline int GetInternalType() { return mInternalType; }
	inline int GetVal() { return mVal; }
	inline std::string GetText() { return mText; }

	bool HasConstant(); // value stored in val is output to text as 4 byte constant
	bool HasData(); // value in mText is output to data as a data entry
	bool IsValid(); // return false for invalid addresses, broken strings, other checking must be done by instruction

	std::string ToBinary();

	enum Internal_Types {
		ARG_NONE = 0,
		ARG_VM = 0,
		ARG_LABEL, //for jump points
		ARG_LABEL_DEC,
		ARG_DATA, //for data addresses
		ARG_STRING, //for string constants
		ARG_PLUS,
		ARG_MINUS,
	};

	void Dump();

private:

	void ParseAddress();
	int mType;
	int mInternalType;
	int mVal;

	bool mValid;

	std::string mText;

	std::vector<Argument*> mSubArguments; //used for multi-part addresses
};

#endif
