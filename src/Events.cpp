#include "Core.hpp"
#include "Events.hpp"

struct Event Event;

void Event::Init() {
	SDL_GetWindowSize(Core.window, &win_w, &win_h);
	SDL_GetMouseState(&mouse_x, &mouse_y);
	SDL_GetRelativeMouseState(&mouse_dx, &mouse_dy);
}

void Event::EnableTextInput() {
	text_input_enabled = true;
	SDL_StartTextInput();
}

void Event::DisableTextInput() {
	text_input_enabled = false;
	SDL_StopTextInput();
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
	switch (event.key.keysym.sym) {
	case SDLK_RETURN:
		Core.ConsoleEnter();
		break;
	case SDLK_BACKSPACE:
		Core.Console.Backspace();
		break;
	case SDLK_LEFT:
		Core.Console.Left();
		break;
	case SDLK_RIGHT:
		Core.Console.Right();
		break;
	}
}

void Event::HandleKeyUpEvent() {
	switch (event.key.keysym.sym) {

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
