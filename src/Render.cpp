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

void Render::RenderTexture(const std::string& img_name, SDL_Rect* src, SDL_Rect* dest) {
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;

	SDL_RenderCopy(Core.renderer, texture, src, dest);
}
