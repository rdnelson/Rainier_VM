%{
#include <stdio.h>
#define YYSTYPE char* 
	extern int yylex();
	void yyerror(const char *s) { printf("ERROR: %s\n", s); }
	void yywrap() {}
%}

%token IDENTIFIER INTVAL LONGVAL FLOATVAL STRING UNSIGNED
%token EQUALS NOTEQUAL GREATER LESS GREQ LEQ AND OR XOR NOT BITAND BITOR BITNOT EQUALTO
%token LPAREN LBRACE LBRAC RPAREN RBRACE RBRAC
%token PLUS MINUS DIVIDE STAR SEMICOLON COMMA COLON
%token INT LONG FLOAT DOUBLE VOID CHAR
%token IF ELSE WHILE DO SWITCH FOR CASE

%start program

%right EQUALS
%left AND OR NOT
%left GREQ LEQ GREATER NOTEQUAL EQUALTO LESS
%left PLUS MINUS
%left STAR DIVIDE
%left BITAND BITOR BITNOT XOR



%%

program: functions					{ printf("Program\n"); }
		;

statements: statement
		|	statements statement { printf("Statements\n"); }
		;

functions: func_decl				{ printf("Functions!\n"); }
		| functions func_decl
		;

statement: expr SEMICOLON	{ printf("Expression Statement: %s\n", $1); }
		| var_decl SEMICOLON		{ printf("Variable declaration: %s\n", $1); }
		| if_struct			{ printf("If structure: %s\n", $1); }
		| while_loop		{ printf("While loop: %s\n", $1); }
		| do_loop			{ printf("Do loop: %s\n", $1); }
		| switch_stmt		{ printf("Switch: %s\n", $1); }
		| for_loop			{ printf("For Loop: %s\n", $1); }
		;

block:	LBRACE statements RBRACE
		| LBRACE RBRACE				{ printf("Empty block\n"); }
		;

var_decl: base_type ident
		| VOID STAR ident
		| base_type ident EQUALS expr
		| VOID STAR ident EQUALS expr
		;

func_decl: base_type ident LPAREN func_args RPAREN block 
		| VOID STAR ident LPAREN func_args RPAREN block
		| VOID ident LPAREN func_args RPAREN block
		| base_type ident LPAREN RPAREN block				{ printf("No Argument Function\n"); }
		| VOID STAR ident LPAREN RPAREN block
		| VOID ident LPAREN RPAREN block
		;

func_args: var_decl
		| func_args COMMA var_decl
		;

ident: IDENTIFIER
		;

base_type: INT
		| LONG
		| FLOAT
		| DOUBLE
		| CHAR
		| UNSIGNED INT
		| UNSIGNED LONG
		| UNSIGNED CHAR
		| base_type STAR
		| base_type LBRAC RBRAC
		;

expr_base: ident EQUALS expr_base
		| ident EQUALS term
		| ident LPAREN call_args RPAREN
		| NOT expr_base
		| NOT term
		| expr_base compare term
		| expr_base math term
		| term compare term
		| term math term
		| LPAREN expr_base RPAREN
		| LPAREN term RPAREN
		;

term: ident
		| numeric
		| STRING
		;

expr: term
		| expr_base
		;

call_args: expr
		| call_args COMMA expr
		;

compare: EQUALTO
		| LESS
		| GREATER
		| GREQ
		| LEQ
		| NOTEQUAL
		| AND
		| OR
		;

math: STAR
		| DIVIDE
		| PLUS
		| MINUS
		| BITAND;
		| BITOR;
		| BITNOT;
		| XOR;
		;

if_stmt: IF LPAREN expr RPAREN block
		;

if_struct: if_stmt
		| if_stmt ELSE block
		;

while_loop: WHILE LPAREN expr RPAREN block
		| WHILE LPAREN expr RPAREN SEMICOLON
		|  WHILE LPAREN expr RPAREN statement
		;

do_loop: DO block WHILE LPAREN expr RPAREN SEMICOLON
		;

switch_stmt: SWITCH LPAREN ident RPAREN LBRACE cases RBRACE
		;

case: CASE numeric COLON statements
		;

cases: case
		| cases case
		;

numeric: INTVAL
		| LONGVAL
		| FLOATVAL
		;

for_loop: FOR LPAREN for_arg SEMICOLON for_arg SEMICOLON for_arg  RPAREN block
		| FOR LPAREN for_arg SEMICOLON for_arg SEMICOLON for_arg  RPAREN statement
		| FOR LPAREN for_arg SEMICOLON for_arg SEMICOLON for_arg  RPAREN SEMICOLON
		;

for_arg: expr
		| for_arg COMMA expr
		;

%%
