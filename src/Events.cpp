#include "Core.hpp"
#include "Events.hpp"

struct Event Event;

double Keypress::GetDuration() {
	return timer.GetTime().count() / 1000000000.0;
}

void Event::Init() {
	SDL_GetWindowSize(Core.window, &win_w, &win_h);
	SDL_GetMouseState(&mouse_x, &mouse_y);
	SDL_GetRelativeMouseState(&mouse_dx, &mouse_dy);
}


void Event::Update() {
	for (auto k = Keypresses.begin(); k != Keypresses.end();) {
		if (k->state == KEY_UP) {
			Keypresses.erase(k);
			continue;
		}

		if (k->state == KEY_DOWN)
			k->state = KEY_HELD;
		++k;
	}
}

Keypress_State Event::GetKey(SDL_Keycode c) {
	for (auto k : Keypresses) {
		if (k.code == c)
			return k.state;
	}
	return KEY_NULL;
}

double Event::GetKeyDuration(SDL_Keycode c) {
	for (auto k : Keypresses) {
		if (k.code == c)
			return k.GetDuration();
	}
	return 0.0;
}

void Event::EnableTextInput() {
	text_input_enabled = true;
	SDL_StartTextInput();
}

void Event::DisableTextInput() {
	text_input_enabled = false;
	SDL_StopTextInput();
}

void Event::EnableRepeatedKeys() {
	repeated_keys = true;
}

void Event::DisableRepeatedKeys() {
	repeated_keys = false;
}

void Event::HandleEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			HandleQuitEvent();
			break;
		case SDL_WINDOWEVENT:
			HandleWindowEvent();
			break;
		case SDL_KEYDOWN:
			HandleKeyDownEvent();
			break;
		case SDL_KEYUP:
			HandleKeyUpEvent();
			break;
		case SDL_MOUSEBUTTONDOWN:
			HandleMouseDownEvent();
			break;
		case SDL_MOUSEBUTTONUP:
			HandleMouseUpEvent();
			break;
		case SDL_MOUSEMOTION:
			HandleMouseMotionEvent();
			break;
		case SDL_TEXTINPUT:
			HandleTextInputEvent();
			break;
		}
	}
}

void Event::HandleQuitEvent() {
	Core.going = false;
}

void Event::HandleWindowEvent() {
	SDL_GetWindowSize(Core.window, &win_w, &win_h);
}

void Event::HandleKeyDownEvent() {
	if (SDL_IsTextInputActive()) {
		bool found = false;
		for (auto k : Core.Console.CONSOLE_KEYS) {
			if (event.key.keysym.sym == k) {
				found = true;
				break;
			}
		}
		if (!found) return;
	}

	// check for repeated input
	for (auto k = Keypresses.begin(); k != Keypresses.end(); ++k) { 
		if (k->code == event.key.keysym.sym) {
			if (repeated_keys)
				k->state = KEY_DOWN;
			return;
		}
	}

	Keypresses.emplace_back(event.key.keysym.sym, KEY_DOWN);
}

void Event::HandleKeyUpEvent() {
	for (auto k = Keypresses.begin(); k != Keypresses.end(); ++k) {
		if (k->code == event.key.keysym.sym) {
			k->state = KEY_UP;
			break;
		}
	}
}

void Event::HandleMouseDownEvent() {
	//
}

void Event::HandleMouseUpEvent() {
	//
}

void Event::HandleMouseMotionEvent() {
	SDL_GetMouseState(&mouse_x, &mouse_y);
	SDL_GetRelativeMouseState(&mouse_dx, &mouse_dy);
}

void Event::HandleTextInputEvent() {
	Core.Console.TextInput(std::string(event.text.text));
}
