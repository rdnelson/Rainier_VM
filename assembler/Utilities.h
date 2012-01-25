#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define WHITE " 	,"
#define IFOP(a) if(!strcmp(a, op))

char* tokenize(char * str, const char* delim);
int GetArgType(char* op);
int GetAddrType(char *op);
int isHex(char *op);
int GetRegister(char *op);

#endif