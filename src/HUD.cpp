#include "HUD.hpp"
#include "Game.hpp"

struct HUD HUD;

void HUD::Update() {
	for (auto h = elements.begin(); h != elements.end();) {
		if ((*h)->destroy) {
			elements.erase(h);
		} else {
			(*h)->Update();
			++h;
		}
	}
}

void HUD::Render() {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Render();
	}

}

void HUD::Clear() {
	elements.clear();
}

void HUD::Add(std::unique_ptr<HUD_Element> element) {
	elements.push_back(std::move(element));
}

namespace HUD_Elements {

void HP_Bar::Update() {
	auto ent = Game.GetEntityByID(ENT_ID);
	if (ent == nullptr) {
		destroy = true;
		return;
	}

	pos = ent->Hull().Middle();
	pos.y = ent->pos.y - 15.0;
	ratio = ent->hitpoints / (double)ent->max_hitpoints;
}

void HP_Bar::Render() {
	Rect rect;
	rect.x = pos.x - size.x/2;
	rect.y = pos.y - size.y;

	rect.w = size.x;
	rect.h = size.y;

	Rect top_rect = rect;
	top_rect.w *= ratio;

	SDL_Color e = {0xff,0x10,0x10,0xff};
	SDL_Color f = {0x10,0xff,0x10,0xff};
	
	SDL_Color c = Renderer.HSLToRGB(ratio/3.0, 1.0, 0.5);

	Renderer.RenderFillRect(    rect, {0x40,0x00,0x00,0xff});
	Renderer.RenderFillRect(top_rect, c);
}

};
