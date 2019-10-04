#pragma once

#include <SDL2/SDL.h>

#include <map>
#include <string>

#include "Events.hpp"

#define PLAYER_LEFT  "+left"
#define PLAYER_RIGHT "+right"
#define PLAYER_JUMP  "+jump"
#define PLAYER_FIRE  "+fire"

#define PLAYER_WEP0 "+slot0"
#define PLAYER_WEP1 "+slot1"
#define PLAYER_WEP2 "+slot2"
#define PLAYER_WEP3 "+slot3"
#define PLAYER_WEP4 "+slot4"
#define PLAYER_WEP5 "+slot5"
#define PLAYER_WEP6 "+slot6"
#define PLAYER_WEP7 "+slot7"
#define PLAYER_WEP8 "+slot8"
#define PLAYER_WEP9 "+slot9"

extern struct Keys {
	void DefaultBindings();

	std::map<std::string, SDL_Keycode> Bindings;

	Keypress_State GetKeyState(const std::string& str);
	double GetKeyDuration(const std::string& str);

	// returns SDLK_UNKNOWN if invalid string
	SDL_Keycode GetKeyFromString(const std::string& str);
	std::string GetStringFromKey(SDL_Keycode key);

	// returns SDLK_UNKNOWN if key is unbound
	SDL_Keycode GetBoundKey(const std::string& str);

	void SetBoundKey(const std::string& str, SDL_Keycode code);
	void UnbindBoundKey(SDL_Keycode code);

	// returns 1 for success, 0 if keyname is an invalid key
	int SetBoundKeyFromString(const std::string& str, const std::string& keyname);
	int UnbindBoundKeyFromString(const std::string& keyname);
} Keys;
