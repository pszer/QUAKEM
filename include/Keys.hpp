#pragma once

#include <SDL2/SDL.h>

#include <map>
#include <string>

#include "Events.hpp"

#define PLAYER_LEFT  "+left"
#define PLAYER_RIGHT "+right"
#define PLAYER_JUMP  "+jump"

extern struct Keys {
	void DefaultBindings();

	std::map<std::string, SDL_Keycode> Bindings;

	Keypress_State GetKeyState(const std::string& str);

	// returns SDLK_UNKNOWN if invalid string
	SDL_Keycode GetKeyFromString(const std::string& str);

	// returns SDLK_UNKNOWN if key is unbound
	SDL_Keycode GetBoundKey(const std::string& str);

	void SetBoundKey(const std::string& str, SDL_Keycode code);

	// returns 1 for success, 0 if keyname is an invalid key
	int SetBoundKeyFromString(const std::string& str, const std::string& keyname);
} Keys;
