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

		Event.Update();
	}
}

void Core::MainRender() {
	Renderer.Clear();

	ConsoleRender();

	Renderer.Update();
}

void Core::Command(const std::string& command) {
	struct Command com;
	if (!Parser::ParseCommand(command, com)) {
		Log::Add(Parser::ErrorMsg);
	} else {
		std::string str = Commands::Execute(com); 
		Log::Add(str);
	}
}


