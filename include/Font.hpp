#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum FONT_TYPE { FONT_GLYPH , FONT_TTF };
struct Font {
	Font(TTF_Font * _ttf, int _size)
	  : type(FONT_TTF), ttf(_ttf). size(_size) {}
	Font(SDL_Texture * _glyph, int size)
	  : type(FONT_GLYPH), glyph(_glyph), size(_size) {}

	const FONT_TYPE type;
	union {
		TTF_Font * ttf;
		SDL_Texture * glyph;
	};
	const int size;
};
