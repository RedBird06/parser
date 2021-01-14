#pragma once
#include "lexer.h"
#include <vector>
#include <map>

struct condition
{
	char v1_name = ' ';
	char v2_name = ' ';
	int v1_val = 0;
	int v2_val = 0;
	bool v1_flag = false;
	bool v2_flag = false;
};

class parser
{
private:
	lexer _lexer;
	std::vector<condition> _conds;
	std::vector<condition>::iterator _conds_pos;
	std::map<char, int> _vars;

	bool _num_flag;
	bool _rec_flag;
	bool _print_plus_flag;

	int cycle();
	int cond();
	int stmt(bool* var_flag);
	int var();

	int Error(std::string errorMessage);
	void clean();

	int _parser_print();
	int _recursive_while(char print_var);
	bool isInf();
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