#include "Keys.hpp"

struct Keys Keys;

void Keys::DefaultBindings() {
	SetBoundKey(PLAYER_LEFT , SDLK_a);
	SetBoundKey(PLAYER_RIGHT, SDLK_d);
	SetBoundKey(PLAYER_JUMP , SDLK_SPACE);
}

Keypress_State Keys::GetKeyState(const std::string& str) {
	SDL_Keycode code = GetBoundKey(str);
	if (code == SDLK_UNKNOWN) return KEY_NULL;
	return Event.GetKey(code);
}

SDL_Keycode Keys::GetKeyFromString(const std::string& str) {
	return SDL_GetKeyFromName(str.c_str());
}

SDL_Keycode Keys::GetBoundKey(const std::string& str) {
	auto b = Bindings.find(str);
	if (b == Bindings.end()) return SDLK_UNKNOWN;
	return b->second;
}

void Keys::SetBoundKey(const std::string& str, SDL_Keycode code) {
	Bindings[str] = code;
}

int Keys::SetBoundKeyFromString(const std::string& str, const std::string& keyname) {
	SDL_Keycode code = GetKeyFromString(keyname);
	if (code == SDLK_UNKNOWN) return 0;
	SetBoundKey(str, code);
	return 1;
}
