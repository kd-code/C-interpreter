#include <string>
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
