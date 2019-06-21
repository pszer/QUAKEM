#pragma once

#include "Cmd.hpp"

// COMMANDS
using CMD_FUNC = std::string(*)(const std::vector<Argument>&);

namespace Commands {

	extern std::map<std::string, CMD_FUNC> COMMANDS;

	void Init();

	CMD_FUNC GetCommand(const std::string& id); // returns nullptr if no command found
	std::string Execute(const struct Command& com);

	// echos its arguments as strings
	// echo ...
	std::string _echo(const std::vector<Argument>&); 

	// quits game
	std::string _quit(const std::vector<Argument>&);

	// sets a consolve variable to some value
	// set $var val
	std::string _set(const std::vector<Argument>&);

	// prints current tick
	std::string _tick(const std::vector<Argument>&);
};
