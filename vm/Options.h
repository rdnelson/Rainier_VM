/*
 * Options.h
 *
 *  Created on: 2012-01-11
 *      Author: daishi
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

class Options {
public:
	Options(int argc, char* argv[]);
	virtual ~Options();

	inline bool IsValid() { return mValid; }

	inline std::string GetExe() { return mExe; }

private:
	bool mValid;
	std::string mExe;
};

#endif /* OPTIONS_H_ */
