#include "HUD.hpp"
#include "Game.hpp"
#include "Core.hpp"
#include "Events.hpp"

struct HUD HUD;

void HUD::Update() {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Update();
	}

	for (auto h = elements.begin(); h != elements.end();) {
		if ((*h)->destroy) {
			elements.erase(h);
		} else ++h;
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
	} else if (ent->destroy) {
		destroy = true;
		return;
	}

	pos = ent->Hull().Middle();
	pos.y = ent->pos.y - 15.0;
	ratio = ent->hitpoints / (double)ent->max_hitpoints;
	if (ratio < 0.0) ratio = 0.0;
}

void HP_Bar::Render() {
	Vec2 tr = Vec2(pos.x, pos.y);
	tr = Renderer.TransformVec2(tr);

	Rect rect;
	rect.x = tr.x * Renderer.zoom - size.x/2;
	rect.y = tr.y * Renderer.zoom - size.y;
	//rect.x = pos.x - size.x/2;
	//rect.y = pos.y - size.y;

	rect.w = size.x;
	rect.h = size.y;

	Rect top_rect = rect;
	top_rect.w *= ratio;

	SDL_Color e = {0xff,0x10,0x10,0xff};
	SDL_Color f = {0x10,0xff,0x10,0xff};
	
	SDL_Color c = Renderer.HSLToRGB(ratio/3.0, 1.0, 0.5);

	Renderer.CameraStop();
	Renderer.RenderFillRect(    rect, {0x40,0x00,0x00,0xff});
	Renderer.RenderFillRect(top_rect, c);
	Renderer.CameraUpdate();
}

void Ammo_Counter::Update() {
	// count = -2 flag = render blank counter
	auto ent = Game.GetEntityByID(ENT_ID);
	if (ent == nullptr) {
		count = -2;

		for (auto e = Game.Entities.begin(); e != Game.Entities.end(); ++e) {
			if ((*e)->type == ENT_PLAYER) {
				ENT_ID = (*e)->UNIQUE_ID;
				return;
			}
		}

		return;
	}

	auto wep = ent->GetWeapon(SLOT);
	if (wep == nullptr) {
		count = -2;
	}

	int ammo = (int)wep->GetKey("ammo");
	if (ammo < 0)
		count = -1;
	else
		count = ammo;
}

void Ammo_Counter::Render() {
	const SDL_Color bg = {54,54,54,0xff};
	const SDL_Color fg = {0xff,0xff,0xff,0xff};

	Renderer.CameraStop();

	Rect rect;
	Vec2 p = PosToScreen(pos);
	rect.x = p.x + offset.x;
	rect.y = p.y + offset.y;
	rect.w = size.x;
	rect.h = size.y;

	Renderer.RenderFillRect(rect, bg);

	Rect icon_rect;
	icon_rect.x = rect.x + 8;
	icon_rect.y = rect.y + 8;
	icon_rect.w = 32;
	icon_rect.h = 32;

	Renderer.RenderTexture(icon, nullptr, &icon_rect);

	if (count == -1) {
		icon_rect.x += 32.0;
		Renderer.RenderTexture("icon_infinite.png", nullptr, &icon_rect);
	} else {
		SDL_Color c = {0xff,0xff,0xff,0xff};

		std::string txt = "-";
		if (count != -2) {
			if (count < 999) {
				txt = std::to_string(count);
			} else {
				txt = "999";
				c = Renderer.HSLToRGB((SDL_GetTicks()%2000)/2000.0, 1.0, 0.5);
			}
		}

		Renderer.RenderText("inconsolata.ttf",txt,rect.x+63,rect.y+4,FONT_P32,
			c,ALIGN_MIDDLE);
	}

	auto ent = Game.GetEntityByID(ENT_ID);
	if (ent) {
		if (ent->active_weapon == SLOT) {
			rect.y += rect.h;
			rect.h  = 5.0;

			Renderer.RenderFillRect(rect, {0xff,0xff,0x00,0xff});
		}
	}

	Renderer.CameraUpdate();
}

};
