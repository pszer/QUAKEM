#pragma once

#include <string>
#include <algorithm>

#include <SDL2/SDL.h>

#include "Log.hpp"
#include "Render.hpp"
#include "Commands.hpp"
#include "Events.hpp"
#include "Music.hpp"
#include "Config.hpp"
#include "Timer.hpp"

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

	bool going = true; // set to false to exit game main loop

	SDL_Window * window   = nullptr;

	// CONSOLE
	struct Console {
		void Toggle();

		std::string font = DEF_CONSOLE_FONT;
		SDL_Color bg = {0x08,0x08,0x08,0xFF},
		          fg = {0xFF,0xFF,0xFF,0xFF};
		bool open = false;
		int bottom_offset = 100; // how many pixels from the bottom
		                         // of the screen console is offset

		int cursor = 0;
		int page_scroll = 0; // how many lines to scroll up
		std::string text = "";

		const int MAX_COMMAND_HISTORY = 64;
		std::vector<std::string> CommandHistory;
		int history_scroll = 0;
		void SetHistoryText();

		void HandleKeypresses();

		void Enter();
		void Reset();
		void Backspace();
		void TextInput(const std::string& input);

		void Left();
		void Right();
		void Up();
		void Down();
		void PageUp();
		void PageDown();
		void Home();
		void End();

		void Render();
		void Update();
	} Console;

	void RenderFPS();

} Core;

