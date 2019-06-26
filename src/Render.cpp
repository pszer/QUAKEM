#include "Core.hpp"
#include "Render.hpp"

struct Renderer Renderer;

void Renderer::SetColor(const SDL_Color& c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
void Renderer::SetColor(const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(renderer, r,g,b,a);
}

void Renderer::Clear(const SDL_Color c) {
	SetColor(c);
	SDL_RenderClear(renderer);
}

void Renderer::Update() {
	SDL_RenderPresent(renderer);
}

void Renderer::RenderPixel(SDL_Point point) {
	SDL_RenderDrawPoint(renderer, point.x, point.y);
}
void Renderer::RenderPixel(SDL_Point point, const SDL_Color& c) {
	SetColor(c);
	SDL_RenderDrawPoint(renderer, point.x, point.y);
}

void Renderer::RenderLine(Vec2 a, Vec2 b) {
	SDL_RenderDrawLine(renderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}
void Renderer::RenderLine(Vec2 a, Vec2 b, const SDL_Color& c) {
	SetColor(c);
	SDL_RenderDrawLine(renderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

void Renderer::RenderRect(Rect rect) {
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderDrawRect(renderer, &r);
}
void Renderer::RenderRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderDrawRect(renderer, &r);
}

void Renderer::RenderFillRect(Rect rect) {
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderFillRect(renderer, &r);
}
void Renderer::RenderFillRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderFillRect(renderer, &r);
}

void Renderer::RenderTexture(const std::string& img_name, SDL_Rect* src, SDL_Rect* dest) {
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;
	SDL_RenderCopy(renderer, texture, src, dest);
}

void Renderer::RenderTexture(const std::string& img_name, SDL_Rect* src, SDL_Rect* dest,
  double angle, SDL_RendererFlip flip, SDL_Point * rot_centre)
{
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;
	SDL_RenderCopyEx(renderer, texture, src, dest, angle, rot_centre, flip);
}

void Renderer::RenderTiledTexture(const std::string& tex_name, Rect _rect, Vec2 scale, Vec2 offset) {
	auto texture = Media.GetTexture(tex_name);
	if (texture == nullptr) return;

	Rect rect = _rect.Absolute();

	int tex_w, tex_h;
	SDL_QueryTexture(texture, nullptr, nullptr, &tex_w, &tex_h);

	// scale cant be 0
	if (scale.x == 0.0 || scale.y == 0.0) return;

	// make scale absolute but flip texture as appropiate
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (scale.x < 0.0) {
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
		scale.x *= -1.0;
	}
	if (scale.y < 0.0) {
		flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
		scale.y *= -1.0;
	}

	double tile_w = tex_w * scale.x,
	       tile_h = tex_h * scale.y;

	offset.x -= rect.x;
	offset.y -= rect.y;
	// wrap offset around and make negative
	offset.x = std::fmod(offset.x, tile_w);
	if (offset.x > 0.0) offset.x -= tile_w;

	offset.y = std::fmod(offset.y, tile_h);
	if (offset.y > 0.0) offset.y -= tile_h;

	SDL_Rect viewport = rect.ToSDLRect();
	SDL_RenderSetViewport(renderer, &viewport);

	SDL_Rect draw_rect = { 0, 0, (int)(tile_w+0.5), (int)(tile_h+0.5) };

	double edge_x = rect.x + rect.w,
	       edge_y = rect.y + rect.h;

	for (double x = offset.x; x < edge_x; x += tile_w) {
		for (double y = offset.y; y < edge_y; y += tile_h) {
			draw_rect.x = x;
			draw_rect.y = y;
			SDL_RenderCopyEx(renderer, texture, nullptr, &draw_rect,
			  0.0, nullptr, flip);
		}
	}

	SDL_RenderSetViewport(renderer, nullptr);
}

void Renderer::RenderText(const std::string& font_name, const std::string& text, int x, int y, 
  FONT_SIZE size, SDL_Color c, TEXT_ALIGN align)
{
	auto font = Media.GetFont(font_name);
	if (font == nullptr) return;

	if (font->type == FONT_TTF) {
		SDL_Surface * surface = TTF_RenderUTF8_Blended(font->GetTTFSize(size), text.c_str(), c);
		SDL_Texture * t = SDL_CreateTextureFromSurface(renderer, surface);

		int w,h;
		TTF_SizeText(font->GetTTFSize(size), text.c_str(), &w, &h);

		SDL_Rect r = {x,y,w,h};

		if (align == ALIGN_RIGHT)
			r.x -= w;
		else if (align == ALIGN_MIDDLE)
			r.x -= w/2;

		SDL_RenderCopy(renderer, t, nullptr, &r);

		SDL_FreeSurface(surface);
		SDL_DestroyTexture(t);
	} else {
	
	}
}

void Renderer::RenderText(TTF_Font * font, const std::string& text, int x, int y, SDL_Color c) {
	SDL_Surface * surface = TTF_RenderUTF8_Blended(font, text.c_str(), c);
	SDL_Texture * t = SDL_CreateTextureFromSurface(renderer, surface);

	int w,h;
	TTF_SizeText(font, text.c_str(), &w, &h);

	SDL_Rect r = {x,y,w,h};
	SDL_RenderCopy(renderer, t, nullptr, &r);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(t);

}
