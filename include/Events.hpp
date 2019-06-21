#pragma once

#include <SDL2/SDL.h>

extern struct Event {

	SDL_Event event;
	void HandleEvents();

	int win_w, win_h;
	int mouse_x, mouse_y,
	    mouse_dx, mouse_dy;

	void Init();

	void HandleQuitEvent();
	void HandleWindowEvent();

	void HandleKeyDownEvent();
	void HandleKeyUpEvent();

	void HandleMouseDownEvent();
	void HandleMouseUpEvent();
	void HandleMouseMotionEvent();

	void HandleTextInputEvent();

} Event;
