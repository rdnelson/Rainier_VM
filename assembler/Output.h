#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <string>
#include <map>

void OutputRegister(char *op, std::string &asmout);
void OutputSplitRegister(char *op, char *op2, std::string &asmout);
int OutputConstant(char *op, std::string &asmout);
void OutputConstant(unsigned int val, std::string &asmout);
void OutputId(char *op, std::string &asmout);
void OutputAddress(char *op, std::string &asmout);
int OutputOneArg(char *op, std::string &asmout, unsigned int textpos, unsigned int lineNumber);

struct unknown {
	unsigned int lineNumber;
	unsigned int textpos;
};

extern std::map<std::string, unsigned int> Labels;
extern std::map<std::string, struct unknown> unknownLabels;
extern std::map<std::string, unsigned int> Data;

typedef std::map<std::string, struct unknown>::iterator Label;
typedef std::map<std::string, unsigned int>::iterator DataPoint;

#endif