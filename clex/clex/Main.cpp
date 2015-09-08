#include <iostream>
#include <fstream>
#include "Tokenizer.h";
using std::cout;
void main(int argc, char *argv[])
{
	if (argc != 2)
		cout << "usage";
	
	return;
	std::ifstream ifstream("d:\\test.c");
	std::string str((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>());
	Tokenizer t(str);
	Token tok;
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

	} while (tok.type != Token::TOKEN_TYPE::FINISHED && tok.type != Token::TOKEN_TYPE::ERROR);
	return;
}