#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "Log.hpp"
#include "Render.hpp"
#include "Commands.hpp"
#include "Events.hpp"

#define DEF_WIN_W 800
#define DEF_WIN_H 600
#define DEF_WIN_TITLE "QUAKEM"
#define DEF_RENDER_FLAG SDL_RENDERER_ACCELERATED

#define DEF_CONSOLE_FONT "fnt/inconsolata.ttf"

struct InitParameters {
	int win_w = DEF_WIN_W,
	    win_h = DEF_WIN_H;
	std::string win_title = DEF_WIN_TITLE;
	SDL_WindowFlags win_flags = SDL_WINDOW_SHOWN; 

	SDL_RendererFlags render_flags = DEF_RENDER_FLAG;
};

extern struct Core {
	Core() { ; }

	int  Init(InitParameters p = InitParameters()); // returns 0 on success, -1 for error
	void Quit();

	void HandleEvents();
	void MainLoop();
	void MainRender();

	void Command(const std::string& command);

	bool going = true; // set to false to exit game main loop

	SDL_Window * window   = nullptr;

	struct Console {
		std::string font = DEF_CONSOLE_FONT;
		SDL_Color bg = {0x08,0x08,0x08,0xFF},
		          fg = {0xFF,0xFF,0xFF,0xFF};
		bool open = true;
		int bottom_offset = 100; // how many pixels from the bottom
		                         // of the screen console is offset

		int cursor = 0;
		std::string text = "";

		void Enter();
		void Reset();
		void Backspace();
		void TextInput(const std::string& input);
		void Left();
		void Right();
	} Console;

	void ConsoleUpdate();
	void ConsoleRender();
	void ConsoleEnter();
} Core;

