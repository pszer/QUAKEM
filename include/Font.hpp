#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum FONT_TYPE { FONT_GLYPH , FONT_TTF };
enum FONT_SIZE { FONT_P10 , FONT_P12 , FONT_P16, FONT_P32 , FONT_P48 };

struct Font {
	Font(TTF_Font ** _ttfs)
	  : type(FONT_TTF), ttfs(_ttfs) {}
	Font(SDL_Texture * _glyph)
	  : type(FONT_GLYPH), glyph(_glyph) {}

	const FONT_TYPE type;
	union {
		TTF_Font ** ttfs; // array[5] of TTF_Font's for different font sizes
		SDL_Texture * glyph;
	};

	TTF_Font * GetTTFSize(FONT_SIZE size);

	~Font();
};
