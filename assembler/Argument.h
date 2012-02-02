#ifndef __ARGUMENT_H__
#define __ARGUMENT_H__

class Argument
{
public:
	Argument(std::string & arg)

	inline int GetType() { return mType; }
	inline int GetVal() { return mVal; }

	bool IsData(); // value stored in val does not need resolution
	bool IsRegister(); // value stored needs resolution
	bool IsAddress(); //value stored in val is an address and might need resolution
	bool IsSubcodeAddress(); //value stored needs registers to resolve

private:
	int mType;
	int mVal;

	std::string mText;
}

#endif
