#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "Filesys.hpp"

#define DEF_WIN_W 800
#define DEF_WIN_H 600
#define DEF_WIN_TITLE "QUAKEM"
#define DEF_RENDER_FLAG SDL_RENDERER_ACCELERATED

struct InitParameters {
	int win_w = DEF_WIN_W,
	    win_h = DEF_WIN_H;
	std::string win_title = DEF_WIN_TITLE;
	SDL_WindowFlags win_flags = SDL_WINDOW_SHOWN; 

	SDL_RendererFlags render_flags = DEF_RENDER_FLAG;
};

extern struct Core {
	Core() { ; }

	std::string err_msg = "";

	int  Init(InitParameters p = InitParameters()); // returns 0 on success, -1 for error
	void Quit();

	void HandleEvents();
	void MainLoop();
	void MainRender();

	bool going = true; // set to false to exit game main loop

	SDL_Renderer * renderer = nullptr;
	SDL_Window   * window   = nullptr;
	SDL_Event      event;
} Core;

