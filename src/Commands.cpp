#include <iostream>

#include "Commands.hpp"
#include "Core.hpp"

using namespace Commands;

std::map<std::string, CMD_FUNC> Commands::COMMANDS;

void Commands::Init() {
	COMMANDS["echo"] = _echo;
}

CMD_FUNC Commands::GetCommand(const std::string& id) {
	auto cmd = COMMANDS.find(id);
	if (cmd == COMMANDS.end()) return nullptr;
	return cmd->second;
}

std::string Commands::Execute(const struct Command& com) {
	auto cmd = GetCommand(com.command);
	if (cmd == nullptr) return "Command \"" + com.command + "\" not found";
	return cmd(com.args);
}

// COMMANDS
std::string Commands::_echo(const std::vector<Argument>& args) {
	for (auto arg : args) {
		std::cout << arg.ToString();
	}
	std::cout << std::endl;
	return "";
}

std::string Commands::_quit(const std::vector<Argument>& args) {
	Core.going = false;
	return "";
}
