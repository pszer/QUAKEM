#include <iostream>

#include "Commands.hpp"
#include "Core.hpp"

using namespace Commands;

std::map<std::string, CMD_FUNC> Commands::COMMANDS;

void Commands::Init() {
	COMMANDS["echo"] = _echo;
	COMMANDS["quit"] = _quit;
	COMMANDS["set"] = _set;
	COMMANDS["tick"] = _tick;
}

CMD_FUNC Commands::GetCommand(const std::string& id) {
	auto cmd = COMMANDS.find(id);
	if (cmd == COMMANDS.end()) return nullptr;
	return cmd->second;
}

std::string Commands::Execute(const struct Command& com) {
	auto cmd = GetCommand(com.command);
	if (cmd == nullptr) return "Command \"" + com.command + "\" not found";
	auto result = cmd(com.args);
	return result;
}

// COMMANDS
std::string Commands::_echo(const std::vector<Argument>& args) {
	std::string str = "";
	for (auto arg : args) {
		str += arg.ToString();
	}
	return str;
}

std::string Commands::_quit(const std::vector<Argument>& args) {
	Core.going = false;
	return "Quitting";
}

std::string Commands::_set(const std::vector<Argument>& args) {
	const std::string USE_MSG = "set cvar value - set a console variable to some value";
	if (args.size() < 2) return USE_MSG;
	if (args.at(0).type != ARG_CVAR) return USE_MSG;
	CVARS[args.at(0).str] = args.at(1);
	return "";
}

std::string Commands::_tick(const std::vector<Argument>& args) {
	return std::to_string(SDL_GetTicks());
}
