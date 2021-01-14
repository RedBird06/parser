#include "lexer.h"
#include <cctype>

lexer::lexer(): _lookahead(0), _intvalue(-1000), _varname(' ') {}
lexer::~lexer(){}

int lexer::scan()
{
	//std::cout << "Parsing str: " << current_str << std::endl;
	std::string lastToken = "";
	while (it != current_str.end())
	{
		//std::cout << "Current symbol is: " << *it << std::endl;
		lastToken += *it;
		if (*it == ' ' || *it == '\t' || *it == '\r') { it++; }
		else if (*it == '\n') {
			_lookahead = '\n';
			return _lookahead;
		}
		else if (isdigit(*it))
		{
			int tokenval = *it - '0';
			*it++;
			lastToken += *it;
			while (it != current_str.end() && isdigit(*it))
			{
				tokenval = tokenval * 10 + (*it - '0');
				lastToken += *it;
				it++;
			}
			_lookahead = NUM;
			_intvalue = tokenval;
			//std::cout << "Intvalue: " << _intvalue << std::endl;
			return _lookahead;
		}
		else if (isTerm("while"))
		{
			lastToken += "hile";
			while (it != current_str.end() && (*it == ' ' || *it == '\t' || *it == '\r')) it++;
			if (it == current_str.end()) return Error("Incomplete statement. EOL.");
			if (*it == '(') 
			{
				it++;
				_lookahead = WHILE;
				return _lookahead;
			}
			if (*it != '(')  return Error("Expected \'(\' after while");
		}
		else if (*it == '<') 
		{
			it++;
			_lookahead = LESS;
			return _lookahead;
		}
		else if (*it == ')') 
		{
			while (it != current_str.end() && (*it == ' ' || *it == '\t' || *it == '\r')) it++;
			if (it == current_str.end())
				return Error("EOL");
			it++;

			if (it != current_str.end() && *it == ';')
			{
				it++;
				_lookahead = SEMICOLON;
				return _lookahead;
			}
			else
			{
				if (it == current_str.begin()) return Error("Incomplete statement.");
				_lookahead = CLOSE_BR;
				return _lookahead;
			}

		}
		else if (isTerm("print"))
		{
			lastToken += "rint";
			while (it != current_str.end() && (*it == ' ' || *it == '\t' || *it == '\r')) it++;
			if (it == current_str.end()) return Error("Incomplete statement. EOL.");
			if (*it == '(') 
			{
				it++;
				_lookahead = PRINT;
				return _lookahead;
			}
			else return Error("Expected \'(\' after \'print\'");
		}
		else if (isalpha(*it))
		{
			_varname = *it;
			do it++; while (it != current_str.end() && (*it == ' ' || *it == '\t' || *it == '\r'));
			if (it == current_str.end()) return Error("Incomplete statement. EOL.");
			if (!isalpha(*it) && (*it == ')' || *it == '<' || *it == '+'))
			{
				_lookahead = VAR;
				return _lookahead;
			}
			else return Error("Non-existing token");
		}
		else if (*it == '+')
		{
			it++;
			if (it == current_str.end()) return Error("Incomplete statement. EOL.");
			if (*it == '+')
			{
				it++;
				_lookahead = PLUS;
				return _lookahead;
			}
			else return Error("Non-existing token");
		}
		else if (it == current_str.end()) return 0;
		else return Error("Non-existing token");
	}

	return 0;
}

int lexer::Error(std::string errorMessage) 
{
	std::cout << "[LEXER] "<< errorMessage << "\n";
	_lookahead = 0;
	return -1;
}

bool lexer::isTerm(std::string term)
{
	std::string::iterator prev_pos = it;
	std::string temp = "";
	for (int i = 0; it != current_str.end() && i < term.length() ; ++i)
	{
		temp += *it;
		it++;
	}
	if (temp == term)
	{
		return true;
	}
	else
	{
		it = prev_pos;
		return false;
	}
}

void lexer::showLookahead()
{
	switch (_lookahead)
	{
	case WHILE:
	{
		std::cout << "WHILE\n";
		break;
	}
	case PRINT:
	{
		std::cout << "PRINT\n";
		break;
	}
	case VAR:
	{
		std::cout << "VAR\n";
		break;
	}
	case NUM:
	{
		std::cout << "NUM\n";
		break;
	}
	case PLUS:
	{
		std::cout << "PLUS\n";
		break;
	}
	case LESS:
	{
		std::cout << "LESS\n";
		break;
	}
	case CLOSE_BR:
	{
		std::cout << "CLOSE_BR\n";
		break;
	}
	case SEMICOLON:
	{
		std::cout << "SEMICOLON\n";
		break;
	}
	default:
	{
		std::cout << "ERROR\n";
		break;
	}
	}
}

void lexer::clean()
{
	_lookahead = 0;
	_intvalue = -1000;
	_varname = ' ';
}