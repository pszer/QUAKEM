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

	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ( (initted&flags) != flags) {
		Log::ErrorMix();
		return -1;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		Log::ErrorMix();
		return -1;
	}

	window = SDL_CreateWindow(init.win_title.c_str(), SDL_WINDOWPOS_UNDEFINED,
	  SDL_WINDOWPOS_UNDEFINED, init.win_w, init.win_h, init.win_flags);
	if (window == nullptr) {
		Log::ErrorSDL();
		return -1;
	}

	Renderer.renderer = SDL_CreateRenderer(window, -1, init.render_flags);
	if (Renderer.renderer == nullptr) {
		Log::ErrorSDL();
		return -1;
	}

	Media.LoadMedia();
	Event.Init();
	Commands::Init();

	CVARS["CVARS"] = Argument(ARG_STRING, "Fetched console variable");

	return 0;
}

void Core::Quit() {
	Media.FreeMedia();

	Mix_CloseAudio();
	Mix_Quit();

	TTF_Quit();

	SDL_DestroyRenderer(Renderer.renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Core::MainLoop() {
	bool intro = true;
	while (going) {
		Event.HandleEvents();

		ConsoleUpdate();

		MainRender();
	}
}

void Core::MainRender() {
	Renderer.Clear();

	ConsoleRender();

	Renderer.Update();
}

void Core::ConsoleUpdate() {
	if (Console.open)
		SDL_StartTextInput();
	else
		SDL_StopTextInput();
}

void Core::ConsoleRender() {
	if (!Console.open) return;

	Font * font_struct = Media.GetFont(Console.font);
	if (font_struct == nullptr || font_struct->type != FONT_TTF) return;
	TTF_Font * font = font_struct->GetTTFSize(FONT_P12);

	Rect bg_rect = { 0 , 0 , Event.win_w , Event.win_h - Console.bottom_offset };
	SDL_Rect rect = { 5, Event.win_h - Console.bottom_offset, 0, 0 };
	TTF_SizeText(font, " ", &rect.w, &rect.h);
	rect.y -= rect.h*2 + 5;

	// background
	Renderer.RenderFillRect(bg_rect, Console.bg);

	// render text
	int i = Log::History.size()-1;
	while (rect.y >= 0 && i >= 0) {
		const std::string& str = Log::History.at(i);

		TTF_SizeText(font, str.c_str(), &rect.w, &rect.h);

		Renderer.RenderText(font, str, rect.x, rect.y, Console.fg);

		rect.y -= (rect.h+1);
		--i;
	}

	// render text being typed
	TTF_SizeText(font, Console.text.c_str(), &rect.w, &rect.h);
	rect.x = 2;
	rect.y = Event.win_h - Console.bottom_offset - rect.h - 2;
	Renderer.RenderText(font, Console.text, rect.x, rect.y, Console.fg);

	rect.x += rect.w + 1;
	rect.w = 2;
	Renderer.RenderFillRect(Rect(rect.x,rect.y,rect.w,rect.h), Console.fg);
}
