%top{
#include "Parser.tab.h"
extern "C" int yywrap() { }
}

DIGIT			[0-9]
WHITE			[ \t\n]
LETTER			[a-zA-Z]
LETTERSYM		[a-zA-Z_]
ALPHANUM		[a-zA-Z0-9]
ALPHANUMSYM		[a-zA-Z0-9_-]

%%
{DIGIT}+					return INTVAL;
{DIGIT}+L					return LONGVAL;
{DIGIT}+.{DIGIT}			return FLOATVAL;

"+"							return PLUS;
"-"							return MINUS;
"/"							return DIVIDE;
"*"							return STAR;

"^"							return XOR;
"&"							return BITAND;
"|"							return BITOR;
"~"							return BITNOT;

"="							return EQUALS;

"=="						return EQUALTO;
"<"							return LESS;
">"							return GREATER;
">="						return GREQ;
"<="						return LEQ;
"!="						return NOTEQUAL;

"&&"						return AND;
"||"						return OR;
"!"							return NOT;

"if"						return IF;
"else"						return ELSE;
"while"						return WHILE;
"do"						return DO;
"for"						return FOR;
"switch"					return SWITCH;
"case"						return CASE;

"int"						return INT;
"long"						return LONG;
"unsigned"					return UNSIGNED;
"float"						return FLOAT;
"double"					return DOUBLE;
"char"						return CHAR;
"void"						return VOID;

"{"							return LBRACE;
"}"							return RBRACE;

"("							return LPAREN;
")"							return RPAREN;

"["							return LBRAC;
"]"							return RBRAC;

";"							return SEMICOLON;
","							return COMMA;
":"							return COLON;

"\"[^\"]*\""				return STRING;

{LETTERSYM}{ALPHANUMSYM}*	return IDENTIFIER;

.
"\n"

%%
