#pragma once

#include "Media.hpp"

extern struct Render {
	void SetColor(const SDL_Color& c);
	void SetColor(const int r, const int g, const int b, const int a);

	void Clear(const SDL_Color c = {0x00,0x00,0x00,0xff});
	void Update();

	void RenderTexture(const std::string& img_name, SDL_Rect* src=nullptr, SDL_Rect* dest=nullptr);
} Render;
