#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define WHITE " 	,"
#define IFOP(a) if(!strcmp(a, op))
#define TO_LOWER(op) { int i = 0; while (op[i]) { op[i] = tolower(op[i]); i++; } }

char* tokenize(char * str, const char* const delim, char bracket = '[', const char* const closebracket = "]");
int GetArgType(char* op);
int GetAddrType(char *op);
int isHex(char *op);
int GetRegister(char *op);

#endif
