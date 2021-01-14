#pragma once

#include <iostream>
#include <string>

enum TERM
{
	WHILE = 1,
	PRINT,
	VAR,
	NUM,
	PLUS,
	LESS,
	OPEN_BR,
	CLOSE_BR,
	SEMICOLON
};

class lexer
{
private:
	bool isTerm(std::string term);
	int Error(std::string errorMessage);
public:
	int _lookahead;
	int _intvalue;
	char _varname;
	std::string current_str;
	std::string::iterator it;

	lexer();
	~lexer();

	int scan();
	void showLookahead();
	void clean();
};

/*
--- Грамматика ---
cycle ::=  while ( cond ) cycle
cycle ::=  print ( stmt ) ;
cond ::= stmt < stmt
cond ::= stmt
stmt ::= var
stmt ::= var ++
stmt ::= num
*/