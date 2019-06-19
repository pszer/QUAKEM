#include "Font.hpp"

TTF_Font * Font::GetTTFSize(FONT_SIZE size) {
	return ttfs[size];
}

Font::~Font() {
	if (type == FONT_GLYPH && glyph != nullptr) {
		SDL_DestroyTexture(glyph);
	} else if (type == FONT_TTF && ttfs != nullptr) {
		for (int i = 0; i < 5; ++i)
			TTF_CloseFont(ttfs[i]);
	}
}
