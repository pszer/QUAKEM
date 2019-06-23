#pragma once

#include <vector>

#include <SDL2/SDL.h>

//                  not pressed
enum Keypress_State { KEY_NULL , KEY_DOWN , KEY_HELD , KEY_UP };
struct Keypress {
	Keypress(SDL_Keycode c, Keypress_State s): code(c), state(s) { }
	SDL_Keycode code;
	Keypress_State state;
};

extern struct Event {

	void Init();
	void Update();

	SDL_Event event;
	void HandleEvents();

	std::vector<Keypress> Keypresses;
	Keypress_State GetKey(SDL_Keycode c);

	int win_w, win_h;
	int mouse_x, mouse_y,
	    mouse_dx, mouse_dy;


	void HandleQuitEvent();
	void HandleWindowEvent();

	void HandleKeyDownEvent();
	void HandleKeyUpEvent();

	void HandleMouseDownEvent();
	void HandleMouseUpEvent();
	void HandleMouseMotionEvent();

	void HandleTextInputEvent();

	bool text_input_enabled = false;
	void EnableTextInput();
	void DisableTextInput();

} Event;
