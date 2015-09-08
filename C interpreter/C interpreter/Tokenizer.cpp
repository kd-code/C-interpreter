#include <string>
#include <cctype>
#include <vector>
#include "Tokenizer.h"
#include "common.h"
#define CUR (source.at(position))
#define NEXT (source.at(position+1))
#define LEN (source.length())
bool Tokenizer::isdelim(char c)
{
	if (std::strchr(" !;,+-<>'/*%^=()[].:\n", c) || c == 9 || c == '\r' || c == 0)
		return true;
	return false;
}
void Tokenizer::setLineCol()
{
	line = 1;
	col = 1;
	for (size_t i = 0; i < position; i++)
	{
		if (source.at(i) == '\n')
		{
			line++;
			col = 0;
		}
		col++;
			
	}
}
std::string Tokenizer::generateErrorMessage(std::string message)
{
	setLineCol();
	return stringFormat("[Error] " + message + " at line %d col %d", line, col);
}

	
Tokenizer::Tokenizer(string source)
{
	this->source = source;
	position = 0;
}
void Tokenizer::StartOver()
{
	position = 0;
}
void Tokenizer::setSouce(string source)
{
	this->source = source;
	position = 0;
}
string Tokenizer::getSource()
{
	return this->source;
}
Token Tokenizer::getNextToken()
{
	Token result;
	if (position == LEN)
	{
		result.type = Token::TOKEN_TYPE::FINISHED;
		result.stringRepresentation = "EOL";
		return result;
	}
	do
	{
		//skip over whitespace characters
		while (position < LEN && std::isspace(CUR))
			position++;
		//skip comments
		if (CUR == '/')
		{
			if (NEXT == '*')
			{
				position+=2;
				do
				{
					while (CUR != '*')
						position++;
					position++;
				} while (CUR != '/');
				position++;
			}
			else if (NEXT == '/')
			{
				position += 2;
				while (CUR != '\n' && CUR != '\r')//new line
					position++;
			}
		}
		//skip over whitespace characters after comments
		while (position < LEN && std::isspace(CUR))
			position++;
	} while (CUR == '/' && (NEXT == '/' || NEXT == '*'));
	//end of file
	if (position == LEN )
	{
		result.type = Token::TOKEN_TYPE::FINISHED;
		result.stringRepresentation = "";
		return result;
	}
	//operators
	std::vector<string> operators{	"+", "-", "*", "/", "%", "++", "--", "==",
									"!=", ">", "<", ">=", "<=", "&&", "||", "!",
									"&", "|", "^", "~", "<<", ">>", "=", "+=",
									"-=", "*=", "/=", "%=", ">>=", "<<=", "&=",
									"^=", "|=", };//using initializer lists
	int ind = 0;
	string op = "";
	//while there is a longer operator keep going
	do
	{
		op += CUR;
		position++;

	} while ((std::find(operators.begin(), operators.end(), op) != operators.end()));
	op.pop_back();//remove last character
	position--;
	if (op.length() > 0)
	{
		result.type = Token::TOKEN_TYPE::OPERATOR;
		result.stringRepresentation = op;
		return result;
	}
	//delimiters
	if (std::strchr("(),;{}[].:", CUR))
	{
		result.type = Token::TOKEN_TYPE::DELIMITER;
		result.stringRepresentation = "";
		result.stringRepresentation += CUR;
		position++;
		return result;

	}
	string ligit_escape = "0abfnrtv\\\"?";
	//string literal
	if (CUR == '"')
	{
		string s = "";
		position++;
		bool escaped = false;
				
		while (CUR != '"' || escaped)
		{
			if (CUR == '\n' || CUR == '\r')
			{
				result.type = Token::TOKEN_TYPE::ERROR;
				result.stringRepresentation = generateErrorMessage("New line in string literal");
				return result;
			}
			if (escaped && std::strchr(ligit_escape.c_str(), CUR)==nullptr)
			{
				result.type = Token::TOKEN_TYPE::ERROR;
				result.stringRepresentation = generateErrorMessage("Illegal escape character");
				return result;
			}
			s += CUR;
			if (CUR == '\\')
				escaped = true;
			else
				escaped = false;
			position++;
		}
		position++;
		result.type = Token::TOKEN_TYPE::STRING_LITERAL;
		result.stringRepresentation = s;
		return result;
	}
	//char literal
	if (CUR == '\'')
	{
		position++;
		if (CUR == '\\')
		{
			position++;
		}
		if (NEXT == '\'')
		{
			if (CUR == '\n' || CUR == '\r')
			{
				result.type = Token::TOKEN_TYPE::ERROR;
				result.stringRepresentation = generateErrorMessage("New line in character literal");
				return result;
			}
			else
			{
				result.type = Token::TOKEN_TYPE::CHAR_LITERAL;
				result.stringRepresentation = "";
				result.stringRepresentation += CUR;
				return result;
				position += 2;
			}
		}
		else
		{
			result.type = Token::TOKEN_TYPE::ERROR;
			result.stringRepresentation = generateErrorMessage("Character literal too long");
			return result;
		}
	}
	//number literal
	if (std::isdigit(CUR))
	{
		string n;
		n += CUR;
		position++;
		while (std::isdigit(CUR))
		{
			n += CUR;
			position++;
		}
		result.type = Token::TOKEN_TYPE::NUMBER_LITERAL;
		result.stringRepresentation = n;
		return result;
	}
	//keywords and identifiers
	string word = "";
	if (std::isalpha(CUR) || CUR == '_')
	{
		while (!isdelim(CUR))
		{
			word += CUR;
			position++;
		}
	}
	std::vector<string> keywords{ "auto", "break", "case", "char", "const", "continue", "default",
								"do", "double", "else", "enum", "extern", "float", "for", "goto",
								"if", "int", "long", "register", "return", "short", "signed", "sizeof",
								"static", "struct", "switch", "typedef", "union", "unsigned", "void",
								"volatile", "while" };
	if (word.length() > 0)
	{
		if ((std::find(keywords.begin(), keywords.end(), word) != keywords.end()))//its a keyword
		{
			result.type = Token::TOKEN_TYPE::KEYWORD;
			result.stringRepresentation = word;
			return result;
		}
		else
		{
			result.type = Token::TOKEN_TYPE::IDENTIFIER;
			result.stringRepresentation = word;
			return result;
		}
	}
	result.type = Token::TOKEN_TYPE::ERROR;
	result.stringRepresentation = generateErrorMessage("Unexpected character");
	return result;
}
		
