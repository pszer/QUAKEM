#pragma once

#include <string>
#include <vector>
#include <map>

enum ARG_TYPE { ARG_STRING , ARG_NUMBER , ARG_CVAR };
enum NUM_TYPE { NUM_INT , NUM_FLOAT };
// intermediate number
struct INumber {
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
	Argument(long long i): type(ARG_NUMBER), num(i) { }
	Argument(double d): type(ARG_NUMBER), num(d) { }
	Argument(ARG_TYPE type, const std::string& str);

	ARG_TYPE type;

	union {
		char * string;
		char * cvar;
		INumber num;
	};

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
