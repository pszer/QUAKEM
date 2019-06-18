#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>

namespace Log {
	void Log(const std::string& str);   // shows up as [LOG]: str
	void Error(const std::string& str); // shows up as [ERR]: str
	void ErrorSDL();                    // shows up as [SDL]: SDL_GetError();
	void ErrorTTF();                    // shows up as [TTF]: TTF_GetError();
	void ErrorIMG();                    // shows up as [IMG]: IMG_GetError();
};
