#include "Core.hpp"
#include "Render.hpp"
#include "Events.hpp"

struct Renderer Renderer;

void Renderer::SetColor(const SDL_Color& c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}
void Renderer::SetColor(const int r, const int g, const int b, const int a) {
	SDL_SetRenderDrawColor(renderer, r,g,b,a);
}

void Renderer::CameraUpdate() {
	if (camera == nullptr) {
		SDL_RenderSetScale(renderer, 1.0f, 1.0f);
		pos = Vec2(0.0, 0.0);
		zoom = 1.0;
	} else {
		SDL_RenderSetScale(renderer, camera->zoom, camera->zoom);
		pos = camera->GetPos();
		zoom = camera->zoom;
	}
	win_w_mid = Event.win_w/2.0;
	win_h_mid = Event.win_h/2.0;
}

void Renderer::CameraStop() {
	SDL_RenderSetScale(renderer, 1.0f, 1.0f);
	pos = Vec2(win_w_mid, win_h_mid);
	zoom = 1.0;
}

void Renderer::CameraStopScale() {
	SDL_RenderSetScale(renderer, 1.0f, 1.0f);
}

void Renderer::CameraStartScale() {
	SDL_RenderSetScale(renderer, camera->zoom, camera->zoom);
}

void Renderer::Clear(const SDL_Color c) {
	SetColor(c);
	SDL_RenderClear(renderer);
}

void Renderer::Update() {
	SDL_RenderPresent(renderer);
}

Vec2 Renderer::TransformVec2(const Vec2& vec) {
	return ((vec - pos) * zoom) + Vec2(win_w_mid, win_h_mid) / zoom;
}

Line Renderer::TransformLine(const Line& line) {
	return Line( TransformVec2(line.a) , TransformVec2(line.b) );
}

Rect Renderer::TransformRect(const Rect& rect) {
	Vec2 pos = TransformVec2( Vec2(rect.x, rect.y) );
	Vec2 size = Vec2(rect.w, rect.h) * zoom;
	return Rect(pos, size);
}

Vec2 Renderer::ReverseTransformVec2(const Vec2& vec) {
	return (vec - (Vec2(win_w_mid, win_h_mid))) / zoom + pos;
}

Line Renderer::ReverseTransformLine(const Line& line) {
	return Line( ReverseTransformVec2(line.a) , ReverseTransformVec2(line.b) );
}

Rect Renderer::ReverseTransformRect(const Rect& rect) {
	Vec2 pos = ReverseTransformVec2( Vec2(rect.x, rect.y) );
	Vec2 size = Vec2(rect.w, rect.h) / zoom;
	return Rect(pos, size);
}

double hue2rgb(double p, double q, double t) {
	if(t < 0.0) t += 1.0;
	if(t > 1.0) t -= 1.0;
	if(t < 1.0/6.0) return p + (q - p) * 6 * t;
	if(t < 1.0/2.0) return q;
	if(t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;
	return p;
}

SDL_Color Renderer::HSLToRGB(double h, double s, double l){
	double r, g, b;

	if (s == 0.0) {
		r = l;
		g = l;
		b = l;
	} else {
		double q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
		double p = 2.0 * l - q;
		r = hue2rgb(p, q, h + 1.0/3.0);
		g = hue2rgb(p, q, h);
		b = hue2rgb(p, q, h - 1.0/3.0);
	}

	return {(unsigned char)(r*255.0),(unsigned char)(g*255.0),(unsigned char)(b*255.0),
	  0xff};
}

void Renderer::RenderPixel(Vec2 p) {
	p = TransformVec2(p);
	SDL_RenderDrawPoint(renderer, p.x, p.y);
}
void Renderer::RenderPixel(Vec2 p, const SDL_Color& c) {
	SetColor(c);
	RenderPixel(p);
}

void Renderer::RenderLine(Vec2 a, Vec2 b) {
	a = TransformVec2(a);
	b = TransformVec2(b);
	SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y);
}
void Renderer::RenderLine(Vec2 a, Vec2 b, const SDL_Color& c) {
	SetColor(c);
	RenderLine(a, b);
}

void Renderer::RenderLine(Line line) {
	RenderLine(line.a, line.b);
}

void Renderer::RenderLine(Line line, const SDL_Color& c){
	RenderLine(line.a, line.b, c);
}

void Renderer::RenderRect(Rect rect) {
	rect = TransformRect(rect);
	SDL_Rect sdlrect = rect.ToSDLRect();
	SDL_RenderDrawRect(renderer, &sdlrect);
}
void Renderer::RenderRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	RenderRect(rect);
}

void Renderer::RenderFillRect(Rect rect) {
	rect = TransformRect(rect);
	SDL_Rect sdlrect = rect.ToSDLRect();
	SDL_RenderFillRect(renderer, &sdlrect);
}
void Renderer::RenderFillRect(Rect rect, const SDL_Color& c) {
	SetColor(c);
	RenderFillRect(rect);
}

void Renderer::RenderTexture(const std::string& img_name, Rect* src, Rect* dest) {
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;

	SDL_Rect sdlsrc, sdldest;

	if (src)  sdlsrc  = TransformRect(*src).ToSDLRect();
	if (dest) sdldest = TransformRect(*dest).ToSDLRect();

	SDL_RenderCopy(renderer, texture, src ? &sdlsrc : nullptr, dest ? &sdldest : nullptr);
}

void Renderer::RenderTexture(const std::string& img_name, Rect* src, Rect* dest,
  double angle, SDL_RendererFlip flip, SDL_Point * rot_centre)
{
	auto texture = Media.GetTexture(img_name);
	if (texture == nullptr) return;

	SDL_Rect sdlsrc, sdldest;

	if (src)  sdlsrc  = TransformRect(*src).ToSDLRect();
	if (dest) sdldest = TransformRect(*dest).ToSDLRect();

	SDL_RenderCopyEx(renderer, texture, src ? &sdlsrc : nullptr, dest ? &sdldest : nullptr,
	  angle, rot_centre, flip);
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
	offset.x -= tile_w;

	offset.y = std::fmod(offset.y, tile_h);
	offset.y -= tile_h;

	SDL_Rect viewport = TransformRect(rect).ToSDLRect();
	SDL_RenderSetViewport(renderer, &viewport);

	Rect draw_rect = { 0, 0, (int)(zoom * tile_w + 0.5), (int)(zoom * tile_h + 0.5) };

	for (double x = offset.x; x < rect.w; x += tile_w) {
		for (double y = offset.y; y < rect.h; y += tile_h) {
			draw_rect.x = x * zoom;
			draw_rect.y = y * zoom;

			SDL_Rect r = draw_rect.ToSDLRect();
			if (std::fmod(draw_rect.x,1.0) && x != offset.x) r.w+=1;
			if (std::fmod(draw_rect.y,1.0) && y != offset.y) r.h+=1;

			SDL_RenderCopyEx(renderer, texture, nullptr, &r,
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

		SDL_Rect r = TransformRect(Rect(x,y,w,h)).ToSDLRect();

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
