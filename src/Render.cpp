#include "Core.hpp"
#include "Render.hpp"

struct Render Render;

void Render::SetColor(const SDL_Color& c) {
	SDL_SetRenderDrawColor(Core.renderer, c.r, c.g, c.b, c.a);
}
void Render::SetColor(const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(Core.renderer, r,g,b,a);
}

void Render::Clear(const SDL_Color c) {
	SetColor(c);
	SDL_RenderClear(Core.renderer);
}

void Render::Update() {
	SDL_RenderPresent(Core.renderer);
}

void Render::RenderPixel(SDL_Point point) {
	SDL_RenderDrawPoint(Core.renderer, point.x, point.y);
}
void Render::RenderPixel(SDL_Point point, const SDL_Color& c) {
	SetColor(c);
	SDL_RenderDrawPoint(Core.renderer, point.x, point.y);
}

void Render::RenderLine(Vec2 a, Vec2 b) {
	SDL_RenderDrawLine(Core.renderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}
void Render::RenderLine(Vec2 a, Vec2 b, const SDL_Color& c) {
	SetColor(c);
	SDL_RenderDrawLine(Core.renderer, (int)a.x, (int)a.y, (int)b.x, (int)b.y);
}

void Render::RenderRect(Rect rect) {
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderDrawRect(Core.renderer, &r);
}
void Render::RenderRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderDrawRect(Core.renderer, &r);
}

void Render::RenderFillRect(Rect rect) {
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderFillRect(Core.renderer, &r);
}
void Render::RenderFillRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	SDL_Rect r = {(int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h};
	SDL_RenderFillRect(Core.renderer, &r);
}

void Render::RenderTexture(const std::string& img_name, SDL_Rect* src, SDL_Rect* dest) {
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;
	SDL_RenderCopy(Core.renderer, texture, src, dest);
}

void Render::RenderTexture(const std::string& img_name, SDL_Rect* src, SDL_Rect* dest,
  double angle, SDL_RendererFlip flip, SDL_Point * rot_centre)
{
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;
	SDL_RenderCopyEx(Core.renderer, texture, src, dest, angle, rot_centre, flip);
}
