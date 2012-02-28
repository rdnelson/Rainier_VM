/*
 * Options.h
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

#define PAGE_SIZE	0x400 	//1k
#define MEM_SIZE	0x10000	//64k

class Options {
public:
	Options(int argc, char* argv[]);
	~Options();

	inline bool IsValid() { return mValid; }
	inline bool IsVerbose() { return mVerbose; }
	inline bool IsStepping() { return mStep; }
	inline unsigned int GetMemSize() { return mMemSize; }

	inline std::string GetExe() { return mExe; }

private:
	bool mValid;
	bool mVerbose;
	bool mStep;
	unsigned int mMemSize;
	unsigned int mPageSize;
	std::string mExe;
};

#endif /* OPTIONS_H_ */
