#include <iostream>

#include "Commands.hpp"
#include "Core.hpp"
#include "Game.hpp"
#include "Keys.hpp"

using namespace Commands;

std::map<std::string, CMD_FUNC> Commands::COMMANDS;

void Commands::Init() {
	COMMANDS["echo"] = _echo;
	COMMANDS["clear"] = _clear;
	COMMANDS["quit"] = _quit;
	COMMANDS["help"] = _help;
	COMMANDS["set"] = _set;
	COMMANDS["bind"] = _bind;
	COMMANDS["unbind"] = _unbind;
	COMMANDS["ent_create"] = _ent_create;
	COMMANDS["ent_args"] = _ent_args;
	COMMANDS["ent_list"] = _ent_list;
	COMMANDS["ent_del"] = _ent_del;
	COMMANDS["brush_create"] = _brush_create;
	COMMANDS["brush_list"] = _brush_list;
	COMMANDS["brush_del"] = _brush_del;
	COMMANDS["mouse_del"] = _mouse_del;
	COMMANDS["mouse_move"] = _mouse_move;
	COMMANDS["mouse_pos"] = _mouse_pos;
	COMMANDS["camera"] = _camera;
	COMMANDS["play_wav"] = _play_wav;
	COMMANDS["play_mus"] = _play_mus;
	COMMANDS["stop_mus"] = _stop_mus;
	COMMANDS["list_cvars"] = _list_cvars;
	COMMANDS["list_tex"] = _list_tex;
	COMMANDS["list_fnt"] = _list_fnt;
	COMMANDS["list_wav"] = _list_wav;
	COMMANDS["list_mus"] = _list_mus;
	COMMANDS["list_binds"] = _list_binds;
	COMMANDS["exec"] = _exec;
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

void Commands::CallCommand(const std::string& str) {
	struct Command com;
	if (!Parser::ParseCommand(str, com)) {
		Log::Add(Parser::ErrorMsg);
	} else {
		std::string result = Commands::Execute(com); 
		Log::Add(result);
	}
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
	Core.Console.Reset();
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

std::string _bind(const std::vector<Argument>& args) {
	const std::string USE_MSG = "bind keyname action";
	if (args.size() < 2) return USE_MSG;
	std::string keyname = args.at(0).ToString(),
	            action = args.at(1).ToString();
	if (!Keys.SetBoundKeyFromString(action, keyname)) {
		return "Unknown key \"" + keyname + "\"";
	}
	return "";
}


std::string _unbind(const std::vector<Argument>& args) {
	const std::string USE_MSG = " unbind keyname";
	std::string keyname;
	if (args.size() == 0) return USE_MSG;
	keyname = args.at(0).ToString();
	if (!Keys.UnbindBoundKeyFromString(keyname)) {
		return "Unknown key \"" + keyname + "\"";
	}
	return "";
}

std::string _ent_create(const std::vector<Argument>& args) {
	const std::string USE_MSG = "ent_create ent_type {args}";
	if (args.size() < 1) return USE_MSG;

	std::string ent_str = args.at(0).ToString();
	auto f = Game.STR_TO_ENT_TYPE.find(ent_str);
	if (f == Game.STR_TO_ENT_TYPE.end()) {
		return "Unknown entity type \"" + ent_str + "\"";
	}

	std::vector<Argument> cut_args;
	cut_args.insert(cut_args.begin(), args.begin()+1, args.end());
	if (!Game.CreateEntity(f->second, cut_args)) {
		return "Error creating entity \"" + ent_str + "\"";
	}
	return "";
}

std::string _ent_args(const std::vector<Argument>& args) {
	const std::string USE_MSG = "ent_args ent_type";
	if (args.size() < 1) return USE_MSG;

	std::string ent_str = args.at(0).ToString();
	auto f = Game.STR_TO_ENT_TYPE.find(ent_str);
	if (f == Game.STR_TO_ENT_TYPE.end()) {
		return "Unknown entity type \"" + ent_str + "\"";
	}

	auto m = Game.ENT_CONSTRUCT_MSG.find(f->second);
	if (m == Game.ENT_CONSTRUCT_MSG.end()) {
		return "No construct message for entity \"" + ent_str + "\"";
	}

	return m->second;
}


#define COL(st,width) \
	s=st;\
	if(s.length()>width)s=s.substr(0,width);\
	digits=s.length()-width;\
	while(digits++ <0)s+=' ';\
	str+=s+'|';
std::string _ent_list(const std::vector<Argument>&) {
	std::string str = "ID NUM|        TYPE        |    EXTRA INFO\n";

	for (auto e = Game.Entities.begin(); e != Game.Entities.end(); ++e) {
		// get number of digits in UNIQUE_ID
		int digits=0, temp = (*e)->UNIQUE_ID;
		if (!temp) digits=1;
		else while (temp) {
			temp /= 10;
			digits++;
		}

		digits-=6;
		while (digits++ < 0) str += ' ';
		str += std::to_string((*e)->UNIQUE_ID) + '|';

		std::string ent_name = "ENT_UNKNOWN";
		for (auto s : Game.STR_TO_ENT_TYPE) {
			if (s.second == (*e)->type) {
				ent_name = s.first;
				break;
			}
		}

		std::string s;
		COL(ent_name,20);

		str += (*e)->Info();

		if (e != Game.Entities.end()-1) {
			str += '\n';
		}
	}

	return str;
}

std::string _ent_del(const std::vector<Argument>& args) {
	const std::string USE_MSG = "ent_del unique_id";
	if (args.size() < 1) return USE_MSG;
	int ID = args.at(0).ToInt();

	for (auto e = Game.Entities.begin(); e != Game.Entities.end(); ++e) {
		if ((*e)->UNIQUE_ID == ID) {
			(*e)->destroy = true;
			return "";
		}
	}

	return "No entity with ID " + std::to_string(ID);
}


std::string _camera(const std::vector<Argument>& args) {
	const std::string USE_MSG = "camera [default] [static] [player] [ent] [path]";

	if (args.size() < 1) return USE_MSG;

	std::string mode = args.at(0).ToString();

	if (mode == "default") {

		Game.camera_mode = Game::GAME_CAMERA_DEFAULT;

	} else if (mode == "static") {

		if (args.size() < 4) return "camera static x y zoom";
		Game.camera_mode = Game::GAME_CAMERA_STATIC;
		Game.camera_pos = Vec2( args.at(1).ToFloat() , args.at(2).ToFloat() );
		Game.camera_zoom = args.at(3).ToFloat();

	} else if (mode == "player") {

		if (args.size() < 2) return "camera player zoom";
		Game.camera_mode = Game::GAME_CAMERA_FOLLOW_PLAYER;
		Game.camera_zoom = args.at(1).ToFloat();

	} else if (mode == "ent") {

		if (args.size() < 3) return "camera ent id zoom";
		Game.camera_mode = Game::GAME_CAMERA_FOLLOW_ENT;
		Game.camera_id = args.at(1).ToInt();
		Game.camera_zoom = args.at(2).ToFloat();

	} else if (mode == "path") {

		if (args.size() < 7) return "path startx starty endx endy dur zoom";
		Game.camera_mode = Game::GAME_CAMERA_PATH;
		Game.camera_start = Vec2( args.at(1).ToFloat() , args.at(2).ToFloat() );
		Game.camera_end   = Vec2( args.at(3).ToFloat() , args.at(4).ToFloat() );
		Game.camera_duration = args.at(5).ToFloat();
		Game.camera_zoom = args.at(6).ToFloat();

	} else {
		return "Available camera modes: default, static, player, ent, path";
	}

	Game.CameraUpdate();

	return "";
}

std::string _brush_create(const std::vector<Argument>& args) {
	const std::string USE_MSG = "brush_create brush_type x y w h"
		" texture [scalex] [scaley] [offsetx] [offsety]";
	if (args.size() < 6) return USE_MSG;
	if (args.size() == 7 || args.size() == 9) return USE_MSG;

	BRUSH_TYPE type;
	Rect rect;
	std::string texture = "";
	Vec2 scale = Vec2(1.0,1.0);
	Vec2 offset = Vec2(0.0,0.0);

	auto bt = STR_TO_BRUSH_TYPE.find(args.at(0).ToString());
	if (bt == STR_TO_BRUSH_TYPE.end()) return "Invalid brush type";
	type = bt->second;

	rect.x = args.at(1).ToFloat();
	rect.y = args.at(2).ToFloat();
	rect.w = args.at(3).ToFloat();
	rect.h = args.at(4).ToFloat();
	texture = args.at(5).ToString();

	// if scale arg present
	if (args.size() > 7) {
		scale.x = args.at(6).ToFloat();
		scale.y = args.at(7).ToFloat();

		if (args.size() > 9) {
			offset.x = args.at(8).ToFloat();
			offset.y = args.at(9).ToFloat();
		}
	}

	rect = rect.Absolute();
	Game.World.Brushes.push_back(std::make_unique<Brush>(rect, type,
	                                                     texture, scale, offset));
	return "";
}


std::string _brush_list(const std::vector<Argument>&) {
	std::string str = "INDEX|     X     |     Y     |    W    |    H    |   TYPE   | TEXTURE\n";

	for (std::size_t i=0; i < Game.World.Brushes.size(); ++i) {
		Brush * brush = Game.World.Brushes.at(i).get();

		long long digits=0, temp = i;
		if (!temp) digits=1;
		else while (temp) {
			temp /= 10;
			digits++;
		}

		digits-=5;
		while (digits++ < 0) str += ' ';
		str += std::to_string(i) + '|';

		std::string s;

		#define COL(st,width) \
		s=st;\
		if(s.length()>width)s=s.substr(0,width);\
		digits=s.length()-width;\
		while(digits++ <0)s+=' ';\
		str+=s+'|';

		COL( std::to_string(brush->rect.x), 11 );
		COL( std::to_string(brush->rect.y), 11 );
		COL( std::to_string(brush->rect.w), 9 );
		COL( std::to_string(brush->rect.h), 9 );

		const std::string TYPENAMES[] =
		  {"SOLID","NONSOLID","BACKGROUND","FOREGROUND"};
		COL( TYPENAMES[(int)brush->type], 10);
		str += brush->texture;

		if (i < Game.World.Brushes.size()-1)
			str += '\n';
	}
	return str;
}

std::string _brush_del(const std::vector<Argument>& args) {
	const std::string USE_MSG = "brush_del index";
	if (args.size() < 1) return USE_MSG;
	auto index = args.at(0).ToInt();
	if (index < 0 || index >= Game.World.Brushes.size())
		return "Invalid index";
	Game.World.Brushes.erase(Game.World.Brushes.begin() + index);
	return "";
}

std::string _mouse_del(const std::vector<Argument>& args) {
	const std::string USE_MSG = "mouse_del [ignore_brush/ignore_ent]";

	bool check_brush = true, check_ent = true;
	for (auto a : args) {
		std::string str = a.ToString();
		if (str == "ignore_brush") check_brush = false;
		else if (str == "ignore_ent") check_ent = false;
		else return USE_MSG;
	}

	Vec2 m_pos = Renderer.ReverseTransformVec2(
		Vec2(Event.mouse_x, Event.mouse_y));

	if (check_brush) {
		for (auto b = Game.World.Brushes.begin(); b != Game.World.Brushes.end(); ++b) {
			if (CheckCollision((*b)->rect, m_pos)) {
				Game.World.Brushes.erase(b);
				return "";
			}
		}
	}

	if (check_ent) {
		for (auto e = Game.Entities.begin(); e != Game.Entities.end(); ++e) {
			if (CheckCollision((*e)->Hull(), m_pos)) {
				Game.Entities.erase(e);
				return "";
			}
		}
	}

	return "";
}

std::string _mouse_move(const std::vector<Argument>& args) {
	const std::string USE_MSG = "mouse_move x y [world/relative]";
	if (args.size() < 2) return USE_MSG;
	Vec2 v = Vec2(args.at(0).ToFloat(), args.at(1).ToFloat());

	bool relative = true;
	if (args.size() > 2) {
		std::string str = args.at(2).ToString();
		if (str == "world") relative = false;
		else if (str == "relative") relative = true;
		else return USE_MSG;
	}

	for (auto b = Game.World.Brushes.begin(); b != Game.World.Brushes.end(); ++b) {
		Rect t_rect = Renderer.TransformRect((*b)->rect);
		if (CheckCollision(t_rect, Vec2(Event.mouse_x, Event.mouse_y))) {
			if (relative) {
				(*b)->rect.x += v.x;
				(*b)->rect.y += v.y;
			} else {
				(*b)->rect.x = v.x;
				(*b)->rect.y = v.y;
			}
			return "";
		}
	}

	for (auto e = Game.Entities.begin(); e != Game.Entities.end(); ++e) {
		Rect t_rect = Renderer.TransformRect((*e)->Hull());
		if (CheckCollision(t_rect, Vec2(Event.mouse_x, Event.mouse_y))) {
			if (relative)
				(*e)->pos = (*e)->pos + v;
			else
				(*e)->pos = v;

			return "";
		}
	}

	return "";	
}

std::string _mouse_pos(const std::vector<Argument>&) {
	Vec2 v = Renderer.ReverseTransformVec2(Vec2(Event.mouse_x,Event.mouse_y));

	return "world : " + std::to_string(v.x) + " " + std::to_string(v.y) +
	       "\nscreen: " + std::to_string(Event.mouse_x) + " " +
	         std::to_string(Event.mouse_y);
}

std::string _play_wav(const std::vector<Argument>& args) {
	const std::string USE_MSG = "play_wav chunkname";

	if (args.size() < 1) return USE_MSG;
	std::string chunk_str = args.at(0).ToString();

	auto chunk = Media.GetChunk(chunk_str);
	if (chunk == nullptr) return "sfx chunk \"" + chunk_str + "\" not found";

	Mix_PlayChannel(-1, chunk, 0);

	return "";
}

std::string _play_mus(const std::vector<Argument>& args) {
	const std::string USE_MSG = "play_mus name [volume] [loops]";

	std::string mus_str;
	float volume = 1.0;
	int loops = 1;

	if (args.size() < 1) return USE_MSG;

	// volume arg
	if (args.size() >= 2) {
		if (args.at(1).type != ARG_NUMBER)
			return USE_MSG;
		else volume = args.at(1).ToFloat();
	}

	// loops args
	if (args.size() >= 3) {
		if (args.at(2).type != ARG_NUMBER)
			 return USE_MSG;
		else loops = args.at(2).ToInt();
	}

	if (volume < 0.0) volume = 0.0;
	if (loops < 0) loops = 0;

	mus_str = args.at(0).ToString();
	Music.Play(mus_str, loops);
	Music.Volume(volume);
	return "";
}

template <typename T>
std::string __search__(const std::map<std::string, T>& v, std::string& find) {
	std::string str = "";
	bool filter = !find.empty();

	for (auto f : v) {
		std::string name = f.first;

		if (filter) {
			if (name.length() < find.length()) continue;

			for (int i = 0; i <= name.length() - find.length(); ++i) {
				if (name.substr(i, find.length()) == find) {
					str += name + "\n";
					continue;
				}
			}

		} else {
			str += name + "\n";
		}

	}

	return str;
}

std::string _stop_mus(const std::vector<Argument>&) {
	Music.Stop();
	return "";
}

std::string _list_tex(const std::vector<Argument>& args) {
	std::string find = "";
	if (args.size() != 0)
		find = args.at(0).ToString();
	return __search__(Media.textures, find);
}

std::string _list_fnt(const std::vector<Argument>& args) {
	std::string find = "";
	if (args.size() != 0)
		find = args.at(0).ToString();
	return __search__(Media.fonts, find);

}

std::string _list_wav(const std::vector<Argument>& args) {
	std::string find = "";
	if (args.size() != 0)
		find = args.at(0).ToString();
	return __search__(Media.chunks, find);
}

std::string _list_mus(const std::vector<Argument>& args) {
	std::string find = "";
	if (args.size() != 0)
		find = args.at(0).ToString();
	return __search__(Media.music, find);
}

std::string _list_cvars(const std::vector<Argument>& args) {
	std::string str = "", find = "";
	if (args.size() > 0) find = args.at(0).ToString();
	bool filter = !find.empty();

	for (auto c : CVARS) {
		std::string name = c.first;

		if (filter) {
			if (name.length() < find.length()) continue;

			for (int i = 0; i <= name.length() - find.length(); ++i) {
				if (name.substr(i, find.length()) == find) {
					str += name + " = " + c.second.ToString() + "\n";
					continue;
				}
			}

		} else {
			str += name + " = " + c.second.ToString() + "\n";
		}

	}

	return str;
}

std::string _list_binds(const std::vector<Argument>& args) {
	std::string str = "", find = "";
	if (args.size() > 0) find = args.at(0).ToString();
	bool filter = !find.empty();

	for (auto c : Keys.Bindings) {
		std::string name = c.first;

		if (filter) {
			if (name.length() < find.length()) continue;

			for (int i = 0; i <= name.length() - find.length(); ++i) {
				if (name.substr(i, find.length()) == find) {
					std::string codename(SDL_GetKeyName(c.second));
					str += name + " = " + codename + "\n";
					continue;
				}
			}

		} else {
			std::string codename = Keys.GetStringFromKey(c.second);
			str += name + " = " + codename + "\n";
		}

	}

	return str;
}

std::string _exec(const std::vector<Argument>& args) {
	const std::string USE_MSG = "exec [path]";
	std::string path;
	if (args.size() == 0) return USE_MSG;
	path = args.at(0).ToString();

	if (!Config::ExecFile(path))
		return "Config file \"" + path + "\" not found";
	else
		return "Loaded config file \"" + path + "\"";
}

}


