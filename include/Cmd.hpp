#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>

enum ARG_TYPE { ARG_STRING , ARG_NUMBER , ARG_CVAR };
enum NUM_TYPE { NUM_INT , NUM_FLOAT };
// intermediate number
struct INumber {
	INumber();
	INumber(long long i): type(NUM_INT), __int__(i) { }
	INumber(double d): type(NUM_FLOAT), __double__(d) { }

	NUM_TYPE type;
	union {
		long long __int__;
		double    __double__;
	};

	long long ToInt();
	double    ToFloat();
};

struct Argument {
	Argument(long long i, std::string _label=""): type(ARG_NUMBER), num(i), label(_label) { }
	Argument(double d, std::string _label=""): type(ARG_NUMBER), num(d), label(_label) { }
	Argument(ARG_TYPE type, const std::string& str, std::string _label="");

	ARG_TYPE type;

	union {
		char * string;
		char * cvar;
		INumber num;
	};

	std::string label;

	long long   ToInt();
	double      ToFloat();
	std::string ToString();
	
	~Argument();
};

struct Command {
	Command(const std::string& _command, const std::vector<Argument>& _args):
	  command(_command), args(_args) { }
	std::string command;
	std::vector<Argument> args;
};

// CONSOLE VARIABLES
extern std::map<std::string, Argument> CVARS;
// GET CONSOLVE VARIABLES
Argument * GetCVar(const std::string& id);
long long GetCVarInt(const std::string& id);
double    GetCVarFloat(const std::string& id);
std::string GetCVarString(const std::string& id);

// COMMANDS
using COM_FUNC = std::string(*)(const std::vector<Argument>&);
extern std::map<std::string, COM_FUNC> COMMANDS;
COM_FUNC GetCommand(const std::string& id); // returns nullptr if no command found
std::string ExecuteCommand(const struct Command& com);

namespace Parser {

	// returns 1 on success, 0 on error
	// - on success Argument& arg is set as the constructed command
	int ParseCommand(const std::string& string, Command& com);

	extern INumber l_number;
	extern std::string l_identifier;

	enum TOKEN {
		TOK_EOF = -1,
		TOK_IDENTIFIER = -2,
		TOK_NUMBER = -3,
		TOK_STRING = -4,

		ERROR = -127;
	};

	void SetupLexer(std::string str);
	int GetNextToken();

}
