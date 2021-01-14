#include "parser.h"

/*
--- Грамматика ---
cycle ::=  while ( cond ) cycle
cycle ::=  print ( stmt ) ;
cond ::= stmt < stmt
cond ::= stmt
stmt ::= var
stmt ::= var ++
stmt ::= num

while(a++<5) while (b++<a) print(b);
while(a++<5) while (b++<a) while (c<10) print(c++);

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

*/

parser::parser(): _num_flag(false), _rec_flag(false), _print_plus_flag(false)
{

}

parser::~parser()
{
	//
}

int parser::cycle()
{
	_lexer.scan();
	//_lexer.showLookahead();
	if (_lexer._lookahead == WHILE) 
	{
		cond();
		//_lexer.showLookahead();
		if (_lexer._lookahead == CLOSE_BR) cycle();

	}
	if (_lexer._lookahead == PRINT) 
	{
		bool var_flag = false;
		_lexer.scan();
		stmt(&var_flag);
		//_lexer.showLookahead();
		if (_lexer._lookahead == NUM) { _num_flag = true; _lexer.scan(); }
		if (_lexer._lookahead == PLUS) { _print_plus_flag = true; _lexer.scan(); }
		if (_lexer._lookahead == CLOSE_BR) return Error("[CYCLE] Syntax error. Missing \';\' . Aborted.");
		else if (_lexer._lookahead == SEMICOLON) return _parser_print();
		else
			return Error("[CYCLE] Syntax error. Aborted.");
	}
	return 0;
}

int parser::cond()
{
	bool var_flag = false;
	condition tmp;

	_lexer.scan();
	//_lexer.showLookahead();

	int res1 = stmt(&var_flag);

	if (var_flag)
	{
		tmp.v1_name = _lexer._varname;
		tmp.v1_val = _vars[tmp.v1_name];
		if (res1 == 1)
			tmp.v1_flag = true;
		else if(res1 == 0)
			tmp.v1_flag = false;
		else
			return Error("[COND] Syntax error. Aborted.");

		//if(tmp.v1_flag) std::cout << "FLAG SETTED\n";
	}
	else
	{
		tmp.v1_name = '*';
		tmp.v1_val = res1;
		tmp.v1_flag = false;
	}
	var_flag = false;
	if (_lexer._lookahead != LESS) _lexer.scan();
	
	_lexer.scan();
	//_lexer.showLookahead();

	if (_lexer._lookahead == CLOSE_BR) 
	{ 
		_conds.push_back(tmp); 
		return 0; 
	}

	int res2 = stmt(&var_flag);

	if (var_flag)
	{
		tmp.v2_name = _lexer._varname;
		tmp.v2_val = _vars[tmp.v2_name];
		if (res2 == 1)
			tmp.v2_flag = true;
		else if (res2 == 0)
			tmp.v2_flag = false;
		else
			return Error("[COND] Syntax error. Aborted.");

		//if (tmp.v2_flag) std::cout << "FLAG SETTED\n";
	}
	else
	{
		tmp.v2_name = '*';
		tmp.v2_val = res2;
		tmp.v2_flag = false;
		_lexer.scan();
	}
	_conds.push_back(tmp);
	//_lexer.showLookahead();

	return 0;
}

int parser::stmt(bool* var_flag)
{
	if (_lexer._lookahead == VAR)
	{
		*var_flag = true;
		//std::cout << "VAR " << _lexer._varname << " SETTED\n";
		return var();
	}
	else if (_lexer._lookahead == NUM)
	{
		//std::cout << "NUM " << _lexer._intvalue << " SETTED\n";
		return _lexer._intvalue;
	}
	else
		return Error("[STMT] Syntax error. Aborted.");
}

int parser::var()
{
	if(_vars.find(_lexer._varname) == _vars.end()) 
		_vars[_lexer._varname] = 0;

	_lexer.scan();

	if (_lexer._lookahead == PLUS)
	{
		return 1;
	}
	else if (_lexer._lookahead == LESS)
	{
		return 0;
	}
	else if (_lexer._lookahead == CLOSE_BR || _lexer._lookahead == SEMICOLON)
	{
		return 0;
	}
	else
		return Error("[VAR] Syntax error. Aborted.");
}

void parser::parse(std::string str)
{
	_lexer.current_str = str;
	_lexer.it = _lexer.current_str.begin();
	cycle();

	//for (auto it = _vars.begin(); it != _vars.end(); it++)
	//{
	//	std::cout << it->first << " " << it->second << std::endl;
	//}
	//for (auto it = _conds.begin(); it != _conds.end(); it++)
	//{
	//	std::cout << std::boolalpha << 
	//		"V1 " << it->v1_name << " " << it->v1_val << " " << it->v1_flag << std::endl <<
	//		"V2 " << it->v2_name << " " << it->v2_val << " " << it->v2_flag << std::endl;
	//}

	clean();
}

void parser::clean()
{
	_lexer.clean();
	_vars.clear();
	_conds.clear();
	_num_flag = false;
	_rec_flag = false;
	_print_plus_flag = false;
}

int parser::_parser_print()
{
	if (isInf()) return -1;

	if (_conds.empty())
	{
		//std::cout << "/////////PRINTING/////////" << std::endl;
		if(_num_flag)
			std::cout << _lexer._intvalue << std::endl;
		else
			std::cout << 0 << std::endl;
		//std::cout << "/////////END/////////" << std::endl;
		return 0;
	}
	_conds_pos = _conds.begin();

	//std::cout << "/////////PRINTING/////////" << std::endl;
	
	_recursive_while(_lexer._varname);

	//std::cout << "/////////END/////////" << std::endl;

	return 0;
}

int parser::_recursive_while(char print_var)
{
	condition tmp;
	bool rec_flag = true;

	tmp = *_conds_pos;
	_conds_pos++;
	

	if(tmp.v1_name == '*') return Error("Infinite loop warning. Aborted.");
	else if(tmp.v1_name == ' ') return Error("Syntax error. Aborted.");
	else
	{
		if (tmp.v2_name == '*')
		{
			while (_vars[tmp.v1_name] < tmp.v2_val)
			{
				if (tmp.v1_flag == true)_vars[tmp.v1_name]++;
				if (_conds_pos != _conds.end())
				{
					_recursive_while(print_var);
					if (_rec_flag == true) rec_flag = false;
				}
				else if(rec_flag)
				{
					_rec_flag = true;
					if(_num_flag)
						std::cout << _lexer._intvalue << std::endl;
					else
					{
						std::cout << _vars[print_var] << std::endl;
						if (_print_plus_flag) _vars[print_var]++;
					}

				}
			}
		}
		else
		{
			while (_vars[tmp.v1_name] < _vars[tmp.v2_name])
			{
				if (tmp.v1_flag == true)_vars[tmp.v1_name]++;
				if (tmp.v2_flag == true)_vars[tmp.v2_name]++;
				if (_conds_pos != _conds.end())
				{
					_recursive_while(print_var);
					if (_rec_flag == true) rec_flag = false;
				}
				else if (rec_flag)
				{
					_rec_flag = true;
					if (_num_flag)
						std::cout << _lexer._intvalue << std::endl;
					else
					{
						std::cout << _vars[print_var] << std::endl;
						if (_print_plus_flag) _vars[print_var]++;
					}
				}	
			}
		}
	}

	return 0;
}

bool parser::isInf()
{
	std::vector<condition>::iterator c_it = _conds.begin();
	std::vector < char> fef;
	return false;
}

int main()
{
//	int a = 0;
//	int b = 0;
//	while (a++ < 5) while (b++ < a) printf("%d\n",b);

	parser _parser;
	std::string line("");

	while (line != "stop")
	{
		 std::getline(std::cin, line);
		_parser.parse(line);
		//std::cout << line << std::endl;
	}

    return 0;
}

int parser::Error(std::string errorMessage)
{
	std::cout << "[PARSER] " << errorMessage << "\n";
	return -1;
}