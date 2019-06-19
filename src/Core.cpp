#include "Core.hpp"

struct Core Core;

int Core::Init(InitParameters init) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		Log::ErrorSDL();
		return -1;
	}

	if (TTF_Init() == -1) {
		Log::ErrorTTF();
		return -1;
	}

	window = SDL_CreateWindow(init.win_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
	  SDL_WINDOWPOS_UNDEFINED, init.win_w, init.win_h, init.win_flags);
	if (window == nullptr) {
		Log::ErrorSDL();
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, init.render_flags);
	if (renderer == nullptr) {
		Log::ErrorSDL();
		return -1;
	}

	Media.LoadMedia();

	return 0;
}

void Core::Quit() {
	Media.FreeMedia();

	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Core::HandleEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			going = false;
			break;
		}
	}
}

void Core::MainLoop() {
	while (going) {
		HandleEvents();

		MainRender();
	}
}

void Core::MainRender() {
	Render.Clear();

	Render.Update();
}
