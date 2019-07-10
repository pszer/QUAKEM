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

double Keys::GetKeyDuration(const std::string& str) {
	SDL_Keycode code = GetBoundKey(str);
	if (code == SDLK_UNKNOWN) return 0.0;
	return Event.GetKeyDuration(code);
}

SDL_Keycode Keys::GetKeyFromString(const std::string& str) {
	const std::map<std::string, int> mouse_names{
		std::make_pair("Mouse1", MOUSE1),
		std::make_pair("Mouse2", MOUSE2),
		std::make_pair("Mouse3", MOUSE3),
		std::make_pair("Mouse4", MOUSE4),
		std::make_pair("Mouse5", MOUSE5),
	};
	auto f = mouse_names.find(str);
	if (f != mouse_names.end()) return (SDL_Keycode)f->second;

	return SDL_GetKeyFromName(str.c_str());
}

SDL_Keycode Keys::GetBoundKey(const std::string& str) {
	auto b = Bindings.find(str);
	if (b == Bindings.end()) return SDLK_UNKNOWN;
	return b->second;
}

void Keys::SetBoundKey(const std::string& str, SDL_Keycode code) {
	UnbindBoundKey(code); // stops key being bound multiple times
	Bindings[str] = code;
}

void Keys::UnbindBoundKey(SDL_Keycode code) {
	for (auto b = Bindings.begin(); b != Bindings.end(); ++b) {
		if (b->second == code) {
			Bindings.erase(b);
			return;
		}
	}
}

int Keys::SetBoundKeyFromString(const std::string& str, const std::string& keyname) {
	SDL_Keycode code = GetKeyFromString(keyname);
	if (code == SDLK_UNKNOWN) return 0;
	SetBoundKey(str, code);
	return 1;
}

int Keys::UnbindBoundKeyFromString(const std::string& keyname) {
	SDL_Keycode code = GetKeyFromString(keyname);
	if (code == SDLK_UNKNOWN) return 0;
	UnbindBoundKey(code);
	return 1;
}
