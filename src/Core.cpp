#include "Core.hpp"

struct Core Core;

int Core::Init(InitParameters init) {
	World::Init();
	Commands::Init();
	Game.Init();
	Keys.DefaultBindings();
	
	if (!Config::ExecFile(CONFIG_PATH)) {
		Log::Error("Config file \"" CONFIG_PATH "\" not found");
	} else {
		Log::Log("Loaded config file \"" CONFIG_PATH "\"");
	}

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

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_SetRenderDrawBlendMode(Renderer.renderer, SDL_BLENDMODE_BLEND);

	Media.LoadMedia();
	Event.Init();

	return 0;
}

void Core::Quit() {
	Media.FreeMedia();
	Game.Quit();

	IMG_Quit();

	Mix_CloseAudio();
	Mix_Quit();

	TTF_Quit();

	SDL_DestroyRenderer(Renderer.renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Core::MainLoop() {
	while (going) {
		FrameLimit.SetLimit(GetCVarInt("maxfps"));
		FrameLimit.FrameStart();

		Event.HandleEvents();
		HandleEvents();

		Game.Update();

		Console.Update();

		MainRender();

		Event.Update();

		FrameLimit.FrameEnd();
		FrameLimit.Sleep();
	}
}

void Core::MainRender() {
	Renderer.Clear();

	Game.Render();

	Console.Render();
	RenderFPS();

	Renderer.Update();
}

void Core::HandleEvents() {
	Console.HandleKeypresses();
}

void Core::RenderFPS() {
	if (!GetCVarInt("showfps")) return;
	int fps = FrameLimit.FPS();
	std::string fps_str = std::to_string(fps);

	SDL_Color c;
	if      (fps < 30 ) c = {0xff,0x00,0x00,0xff};
	else if (fps < 90 ) c = {0xff,0xff,0x00,0xff};
	else if (fps < 150) c = {0x00,0xff,0x00,0xff};
	else                c = {0x00,0xff,0xff,0xff};

	Renderer.CameraStop();
	Renderer.RenderText(Console.font, fps_str, Event.win_w, 0, FONT_P16, c, ALIGN_RIGHT);
	Renderer.CameraUpdate();
}
