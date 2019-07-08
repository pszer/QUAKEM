#pragma once

#include <cmath>

#include "Camera.hpp"
#include "Media.hpp"

enum TEXT_ALIGN { ALIGN_LEFT, ALIGN_MIDDLE , ALIGN_RIGHT };

extern struct Renderer {
	SDL_Renderer * renderer = nullptr;
	Camera * camera = nullptr;
	Vec2 pos = Vec2(0.0,0.0);
	double zoom = 1.0;
	double win_w_mid = 400.0, win_h_mid = 300.0;

	void SetColor(const SDL_Color& c);
	void SetColor(const int r, const int g, const int b, const int a);

	void CameraUpdate();
	void CameraStop(); // stops cameras transformations
	void Clear(const SDL_Color c = {0x00,0x00,0x00,0xff});
	void Update();

	Vec2 TransformVec2(const Vec2& vec);
	Line TransformLine(const Line& line);
	Rect TransformRect(const Rect& rect);
	
	void RenderPixel(Vec2 p);
	void RenderPixel(Vec2 p, const SDL_Color&);

	void RenderLine(Vec2 a, Vec2 b);
	void RenderLine(Vec2 a, Vec2 b, const SDL_Color&);
	void RenderLine(Line line);
	void RenderLine(Line line, const SDL_Color&);

	void RenderRect(Rect rect);
	void RenderRect(Rect, const SDL_Color&);

	void RenderFillRect(Rect rect);
	void RenderFillRect(Rect, const SDL_Color&);

	void RenderTexture(const std::string& img_name, Rect* src=nullptr,
	  Rect* dest=nullptr);
	void RenderTexture(const std::string& img_name, Rect* src, Rect* dest,
  	  double angle, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Point * rot_centre = nullptr);

	void RenderTiledTexture(const std::string& tex_name, Rect rect, Vec2 scale, Vec2 offset);

	void RenderText(const std::string& font_name, const std::string& text, int x, int y,
	  FONT_SIZE, SDL_Color, TEXT_ALIGN align=ALIGN_LEFT);
	void RenderText(TTF_Font * font, const std::string& text, int x, int y, SDL_Color);
} Renderer;
