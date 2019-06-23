#include <iostream>

#include "Commands.hpp"
#include "Core.hpp"

using namespace Commands;

std::map<std::string, CMD_FUNC> Commands::COMMANDS;

void Commands::Init() {
	COMMANDS["echo"] = _echo;
	COMMANDS["clear"] = _clear;
	COMMANDS["quit"] = _quit;
	COMMANDS["help"] = _help;
	COMMANDS["set"] = _set;
	COMMANDS["tick"] = _tick;
	COMMANDS["playwav"] = _playwav;
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

namespace Commands {

std::string _echo(const std::vector<Argument>& args) {
	std::string str = "";
	for (auto arg : args) {
		str += arg.ToString();
	}
	return str;
}

std::string _clear(const std::vector<Argument>& args) {
	Log::History.clear();
	return "";
}

std::string _quit(const std::vector<Argument>& args) {
	Core.going = false;
	return "Quitting";
}

std::string _help(const std::vector<Argument>&) {
	std::string help_str = "Available commands:\n";
	for (auto c = COMMANDS.begin(); c != COMMANDS.end(); ++c) {
		help_str += c->first + "\n";
	}
	return help_str;
}

std::string _set(const std::vector<Argument>& args) {
	const std::string USE_MSG = "set cvar value - set a console variable to some value";
	if (args.size() < 2) return USE_MSG;
	if (args.at(0).type != ARG_CVAR) return USE_MSG;
	CVARS[args.at(0).str] = args.at(1);
	return "";
}

std::string _tick(const std::vector<Argument>& args) {
	return std::to_string(SDL_GetTicks());
}

std::string _playwav(const std::vector<Argument>& args) {
	const std::string USE_MSG = "playwav [chunkname]";

	if (args.size() < 1) return USE_MSG;
	std::string chunk_str = args.at(0).ToString();

	auto chunk = Media.GetChunk(chunk_str);
	if (chunk == nullptr) return "sfx chunk \"" + chunk_str + "\" not found";

	Mix_PlayChannel(-1, chunk, 0);

	return "";
}

}
