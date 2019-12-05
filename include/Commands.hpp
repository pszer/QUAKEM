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
	// echo {args}
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

	// binds action to a key
	// bind keyname action
	std::string _bind(const std::vector<Argument>&);

	// unbinds a key
	// unbind keyname
	std::string _unbind(const std::vector<Argument>&);

	// lists binds
	// list_binds [search]
	std::string _list_binds(const std::vector<Argument>& args);

	// creates entity
	// ent_create ent_type {args}
	std::string _ent_create(const std::vector<Argument>&);

	// lists entity construct arguments
	// ent_args ent_type
	std::string _ent_args(const std::vector<Argument>&);

	// lists all spawned entities
	// ent_list
	std::string _ent_list(const std::vector<Argument>&);

	// delete spawned entity by unique id
	// ent_del unique_id
	std::string _ent_del(const std::vector<Argument>&);

	// set camera mode
	// camera [static pos zoom] [player zoom] [ent id zoom] [path start end dur zoom]
	std::string _camera(const std::vector<Argument>&);

	// creates standard brush
	// brush_create brush_type x y w h texture [scalex] [scaley] [offsetx] [offsety] {key:val}
	std::string _brush_create(const std::vector<Argument>&);

	// lists all brushes
	// brush_list
	std::string _brush_list(const std::vector<Argument>&);

	// delete a brush
	// brush_del index	
	std::string _brush_del(const std::vector<Argument>& args);

	// deletes brush/entity pointed at by mouse
	// mouse_del {ignore_brush/ignore_ent}
	std::string _mouse_del(const std::vector<Argument>& args);

	// moves brush/entity pointed at by mouse
	// mouse_move x y [world/relative]
	std::string _mouse_move(const std::vector<Argument>& args);

	// prints mouse position
	std::string _mouse_pos(const std::vector<Argument>&);

	// lists all set cvars
	std::string _list_cvars(const std::vector<Argument>&);

	// plays loaded audio chunk
	// play_wav name
	std::string _play_wav(const std::vector<Argument>&);

	// plays loaded music file
	// play_mus name [volume] [loops]
	std::string _play_mus(const std::vector<Argument>&);

	// stops music
	std::string _stop_mus(const std::vector<Argument>&);

	// lists loaded textures
	// list_tex [search]
	std::string _list_tex(const std::vector<Argument>&);

	// lists loaded fonts
	// list_fnt [search]
	std::string _list_fnt(const std::vector<Argument>&);

	// lists loaded sfx chunks
	// list_wav [search]
	std::string _list_wav(const std::vector<Argument>&);

	// lists loaded music files
	// list_mus [search]
	std::string _list_mus(const std::vector<Argument>&);

	// executes config file
	// exec file
	std::string _exec(const std::vector<Argument>&);

	// exports currently loaded world as a map file
	// export filename
	std::string _export(const std::vector<Argument>&);

	// loads map file : synonymous with exec maps/"filename"
	// map filename
	std::string _map(const std::vector<Argument>&);

	// loads wave file in waves/ folder
	// wave filename
	std::string _wave(const std::vector<Argument>&);

	// delays sequential wave commands
	// delay ms
	std::string _delay(const std::vector<Argument>&);

	// gives 1000 ammo for all weapons
	std::string _dem_gunmad(const std::vector<Argument>&);

	// gives super machine gun
	std::string _bomboclat(const std::vector<Argument>&);
};
