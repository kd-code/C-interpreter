#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#define CUR (source.at(position))
#define NEXT (source.at(position+1))
#define LEN (source.length())
using std::cout;
using std::string;


struct Token
{
	enum class TOKEN_TYPE
	{
		KEYWORD,
		IDENTIFIER,
		OPERATOR,
		DELIMITER,
		CHAR_LITERAL,
		STRING_LITERAL,
		NUMBER_LITERAL,
		FINISHED,
		ERROR,
	};
	TOKEN_TYPE type;
	string stringRepresentation;
	int line;
	int col;
	string getTypeStr()
	{
		switch (type)
		{
			case TOKEN_TYPE::KEYWORD:
				return "KEYWORD";
				break;
			case TOKEN_TYPE::IDENTIFIER:
				return "IDENTIFIER";
				break;
			case TOKEN_TYPE::OPERATOR:
				return "OPERATOR";
				break;
			case TOKEN_TYPE::DELIMITER:
				return "DELIMITER";
				break;
			case TOKEN_TYPE::CHAR_LITERAL:
				return "CHAR_LITERAL";
				break;
			case TOKEN_TYPE::STRING_LITERAL:
				return "STRING_LITERAL";
				break;
			case TOKEN_TYPE::NUMBER_LITERAL:
				return "NUMBER_LITERAL";
			case TOKEN_TYPE::FINISHED:
				return "FINISHED";
				break;
			case TOKEN_TYPE::ERROR:
				return "ERROR";
				break;
		}

	}

};
class Tokenizer
{
	//private data
	private:
		string source;
		unsigned int position;
	//private function
	private:
	bool isdelim(char c)
	{
		if (std::strchr(" !;,+-<>'/*%^=()[].:\n", c) || c == 9 || c == '\r' || c == 0)
			return true;
		return false;
	}

	public:
		Tokenizer(string source)
		{
			this->source = source;
			position = 0;
		}
		void StartOver()
		{
			position = 0;
		}
		void setSouce(string source)
		{
			this->source = source;
			position = 0;
		}
		string getSource()
		{
			return this->source;
		}
		Token getNextToken()
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
						result.stringRepresentation = "New line in string literal";
						return result;
					}
					if (escaped && std::strchr(ligit_escape.c_str(), CUR)==nullptr)
					{
						result.type = Token::TOKEN_TYPE::ERROR;
						result.stringRepresentation = "Illegal escape character";
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
						result.stringRepresentation = "New line in character literal";
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
					result.stringRepresentation = "Character literal too long";
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
			result.stringRepresentation = "Unexpected character";
			return result;
		}
		
};
enum class SYMBOL_TYPE
{
	EXTERN,
	ID,
	VOID,
	LPAR,
	RPAR,
	INTLIT,
	CHARLIT,
	STRLIT,
	CHAR,
	INT,
	LBRAC,
	RBRAC,
	LCURL,
	RCURL,
	IF,
	ELSE,
	WHILE,
	FOR,
	RETURN,
	SEMI,
	COLON,
	COMMA,
	OP,
	EQ,
	MINUS,
	NOT,
	END
};
SYMBOL_TYPE symbol;
Tokenizer *t;
void nextSymbol()
{
	Token tok = t->getNextToken();
	switch (tok.type)
	{
		case Token::TOKEN_TYPE::CHAR_LITERAL:
			symbol = SYMBOL_TYPE::CHARLIT;
			break;
		case Token::TOKEN_TYPE::DELIMITER:
			if (tok.stringRepresentation == "(")
				symbol = SYMBOL_TYPE::LPAR;
			else if (tok.stringRepresentation == ")")
				symbol = SYMBOL_TYPE::RPAR;
			else if (tok.stringRepresentation == "[")
				symbol = SYMBOL_TYPE::LBRAC;
			else if (tok.stringRepresentation == "]")
				symbol = SYMBOL_TYPE::RBRAC;
			else if (tok.stringRepresentation == "{")
				symbol = SYMBOL_TYPE::LCURL;
			else if (tok.stringRepresentation == "}")
				symbol = SYMBOL_TYPE::RCURL;
			else if (tok.stringRepresentation == ";")
				symbol = SYMBOL_TYPE::SEMI;
			else if (tok.stringRepresentation == ":")
				symbol = SYMBOL_TYPE::COLON;
			else if (tok.stringRepresentation == ",")
				symbol = SYMBOL_TYPE::COMMA;
			break;
		case Token::TOKEN_TYPE::FINISHED:
			symbol = SYMBOL_TYPE::END;
			break;
		case Token::TOKEN_TYPE::IDENTIFIER:
			symbol = SYMBOL_TYPE::ID;
			break;
		case Token::TOKEN_TYPE::KEYWORD:
			if (tok.stringRepresentation == "extern")
				symbol = SYMBOL_TYPE::EXTERN;
			else if (tok.stringRepresentation == "void")
				symbol = SYMBOL_TYPE::VOID;
			else if (tok.stringRepresentation == "char")
				symbol = SYMBOL_TYPE::CHAR;
			else if (tok.stringRepresentation == "int")
				symbol = SYMBOL_TYPE::INT;
			else if (tok.stringRepresentation == "if")
				symbol = SYMBOL_TYPE::IF;
			else if (tok.stringRepresentation == "else")
				symbol = SYMBOL_TYPE::ELSE;
			else if (tok.stringRepresentation == "while")
				symbol = SYMBOL_TYPE::WHILE;
			else if (tok.stringRepresentation == "for")
				symbol = SYMBOL_TYPE::FOR;
			else if (tok.stringRepresentation == "return")
				symbol = SYMBOL_TYPE::RETURN;
			break;
		case Token::TOKEN_TYPE::NUMBER_LITERAL:
			symbol = SYMBOL_TYPE::INTLIT;
			break;
		case Token::TOKEN_TYPE::OPERATOR:
			if (tok.stringRepresentation == "-")
				symbol = SYMBOL_TYPE::MINUS;
			else if (tok.stringRepresentation == "!")
				symbol = SYMBOL_TYPE::NOT;
			else if (tok.stringRepresentation == "=")
				symbol = SYMBOL_TYPE::EQ;
			else
				symbol = SYMBOL_TYPE::OP;
			break;
		case Token::TOKEN_TYPE::STRING_LITERAL:
			symbol = SYMBOL_TYPE::STRLIT;
			break;


	}
	
}
void error(string message)
{
	cout << message;
}
int accept(SYMBOL_TYPE s)
{
	if (symbol == s)
	{
		nextSymbol();
		return 1;
	}
	return 0;
}
int expect(SYMBOL_TYPE s)
{
	if (accept(s))
		return 1;
	error("expect: unexpected symbol");
	return 0;
}
/////////////////////////////
//recursive descend parsing//
/////////////////////////////
void var_decl()
{

}
void type()
{

}
void paramTypes()
{

}
int dcl()
{

}
int func()
{

}

void prog()
{
	if (accept(SYMBOL_TYPE::EXTERN))
	{
		if (accept(SYMBOL_TYPE::VOID))
		{
			do
			{
				expect(SYMBOL_TYPE::ID);
				expect(SYMBOL_TYPE::LPAR);
				paramTypes();
				expect(SYMBOL_TYPE::RPAR);
			} while (accept(SYMBOL_TYPE::COMMA));
		}
		else
		{
			type();
			do
			{
				expect(SYMBOL_TYPE::ID);
				expect(SYMBOL_TYPE::LPAR);
				paramTypes();
				expect(SYMBOL_TYPE::RPAR);
			} while (accept(SYMBOL_TYPE::COMMA));
		}
		
	}
	else if (accept(SYMBOL_TYPE::VOID))
	{
		expect(SYMBOL_TYPE::ID);
		expect(SYMBOL_TYPE::LPAR);
		paramTypes();
		expect(SYMBOL_TYPE::RPAR);
		expect(SYMBOL_TYPE::LCURL);
		do
		{
			type();
			do
			{
				var_decl();
			} while (accept(SYMBOL_TYPE::COMMA));
			expect(SYMBOL_TYPE::SEMI);
		} while (accept(SYMBOL_TYPE::INT) || accept(SYMBOL_TYPE::CHAR));
	}
	else
	{
		type();
		expect(SYMBOL_TYPE::ID);
		if (accept(SYMBOL_TYPE::LPAR))
		{
			paramTypes();
			expect(SYMBOL_TYPE::RPAR);
			expect(SYMBOL_TYPE::LCURL);
			do
			{
				type();
				do
				{
					var_decl();
				} while (accept(SYMBOL_TYPE::COMMA));
				expect(SYMBOL_TYPE::SEMI);
			} while (accept(SYMBOL_TYPE::INT) || accept(SYMBOL_TYPE::CHAR));
		}
		else if (accept(SYMBOL_TYPE::LBRAC))
		{
			expect(SYMBOL_TYPE::INTLIT);
			expect(SYMBOL_TYPE::RBRAC);
		}
		else
		{

		}
		
	}

	
}




void main(int argc, char *argv[])
{
	std::ifstream ifstream("d:\\test.c");
	std::string str((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>());
	
	t = new  Tokenizer(str);

	/*Token tok;
	int n = 0;
	do
	{
		tok = t.getNextToken();
		cout << tok.stringRepresentation<<":"<<tok.getTypeStr()<<"\n";
		n++;
		if (n % 10 == 0)
		{
		//	cout << "---";
		}

	} while (tok.type != Token::TOKEN_TYPE::FINISHED && tok.type != Token::TOKEN_TYPE::ERROR);*/
	
	//acutal code

	nextSymbol();
	prog();
	

	return;
}