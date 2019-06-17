#pragma once

#include <string>
#include <vector>
#include <map>

struct Command {
	Command(const std::string& _command);
	std::string command;
	std::vector<Argument> args;
};

enum ARG_TYPE { ARG_STRING , ARG_NUMBER , ARG_CVAR };
struct Argument {
	
};
