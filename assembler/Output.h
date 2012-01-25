#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <string>

void OutputRegister(char *op, std::string &asmout);
void OutputSplitRegister(char *op, char *op2, std::string &asmout);
void OutputConstant(char *op, std::string &asmout);
void OutputAddress(char *op, std::string &asmout);

#endif