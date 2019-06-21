#pragma once

#include "Cmd.hpp"

// COMMANDS
using CMD_FUNC = std::string(*)(const std::vector<Argument>&);

namespace Commands {

	extern std::map<std::string, CMD_FUNC> COMMANDS;

	void Init();

	CMD_FUNC GetCommand(const std::string& id); // returns nullptr if no command found
	std::string Execute(const struct Command& com);

	std::string _echo(const std::vector<Argument>&);
	std::string _quit(const std::vector<Argument>&);

};
