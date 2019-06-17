#include "Core.hpp"

struct Core Core;

int Core::Init(InitParameters init) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		err_msg = std::string(SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow(init.win_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
	  SDL_WINDOWPOS_UNDEFINED, init.win_w, init.win_h, init.win_flags);
	if (window == nullptr) {
		err_msg = std::string(SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, init.render_flags);
	if (renderer == nullptr) {
		err_msg = std::string(SDL_GetError());
		return -1;
	}

	Media.LoadMedia();

	return 0;
}

void Core::Quit() {
	Media.FreeMedia();
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
	Render.RenderTexture("img/scrung.jpg");
	Render.Update();
}
