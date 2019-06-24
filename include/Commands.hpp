#pragma once

#include "Cmd.hpp"

// COMMANDS
using CMD_FUNC = std::string(*)(const std::vector<Argument>&);

namespace Commands {

	extern std::map<std::string, CMD_FUNC> COMMANDS;

	void Init();

	CMD_FUNC GetCommand(const std::string& id); // returns nullptr if no command found
	std::string Execute(const struct Command& com);
	void CallCommand(const std::string& str);

	// echos its arguments as strings
	// echo ...
	std::string _echo(const std::vector<Argument>&); 

	// clears log history
	std::string _clear(const std::vector<Argument>&);

	// prints all command names
	std::string _help(const std::vector<Argument>&);

	// quits game
	std::string _quit(const std::vector<Argument>&);

	// sets a consolve variable to some value
	// set $var val
	std::string _set(const std::vector<Argument>&);

	// prints current tick
	std::string _tick(const std::vector<Argument>&);

	// plays loaded audio chunk
	// playwav [name]
	std::string _playwav(const std::vector<Argument>&);

	// plays loaded music file
	// playmus [name] {volume} {loops}
	std::string _playmus(const std::vector<Argument>&);

	// stops music
	std::string _stopmus(const std::vector<Argument>&);

	// lists loaded textures
	// listtex {search}
	std::string _listtex(const std::vector<Argument>&);

	// lists loaded fonts
	// listfnt {search}
	std::string _listfnt(const std::vector<Argument>&);

	// lists loaded sfx chunks
	// listwav {search}
	std::string _listwav(const std::vector<Argument>&);

	// lists loaded music files
	// listmus {search}
	std::string _listmus(const std::vector<Argument>&);
};
