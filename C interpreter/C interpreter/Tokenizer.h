#include <string>
#include "Token.h"
class Tokenizer
{
	//private data
	private:
		string source; //string containing input source code
		unsigned int position;//currecnt character index in the source code
		unsigned int line;// current line
		unsigned int col;//current column
	//private function
	private:
		bool isdelim(char c);
		void setLineCol();
		std::string generateErrorMessage(std::string message);
	public:
		Tokenizer(string source);
		void StartOver();
		void setSouce(string source);
		string getSource();
		Token getNextToken();
};
