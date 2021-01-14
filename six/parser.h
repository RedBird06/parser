#pragma once
#include "../lexer.h"

class parser
{
private:
	lexer _lexer;
	char _fv;
	int _num_of_repeats;
	bool _inf_flag;
	bool _num_flag;

	int cycle();
	int cond();
	int stmt();
	int var();

	int Error(std::string errorMessage);
	void clean();
	void check_flag();

public:
	parser();
	~parser();

	void parse(std::string str);
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