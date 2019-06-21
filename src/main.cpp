#include "Core.hpp"

#include <iostream>

int main(int argc, char ** argv) {
	std::string com = "test_command 50 128.0 \"hello\"";
	
	Parser::SetupLexer(com);
	while (1) {
		int tok = Parser::GetNextToken();
		if (tok == Parser::TOK_EOF) break;

		switch (tok) {
		case Parser::TOK_STRING:
			std::cout << "TOK_STRING: \"" << Parser::l_string
				<< "\"" << std::endl;
			break;
		case Parser::TOK_IDENTIFIER:
			std::cout << "TOK_IDENTIFIER: " << Parser::l_string
				<< std::endl;
			break;
		case Parser::TOK_NUMBER:
			std::cout << "TOK_NUMBER: " <<
				(Parser::l_number.type == NUM_INT ?
				 "int " + std::to_string(Parser::l_number.ToInt()) : 
				 "double " + std::to_string(Parser::l_number.ToFloat())) << std::endl;
		}
	}

	return 0;
	/*if (Core.Init()) {
		Log::Error("QUITTING");
		Core.Quit();
		return -1;
	} else {
		Core.MainLoop();
		Core.Quit();
		return 0;
	}*/
}
