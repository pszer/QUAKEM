#include "Menu.hpp"
#include "Events.hpp"
#include "Hitreg.hpp"
#include "Render.hpp"

Rect Menu_Element::GetRect() {
	Rect result = Rect(0.0, 0.0, size.x, size.y);

	result.x = pos.x * Event.win_w + offset.x;
	result.y = pos.y * Event.win_h + offset.y;

	return result;
}

void Menu_Screen::Update(Menu * m) {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Update(m);
	}

	for (auto h = elements.begin(); h != elements.end();) {
		if ((*h)->destroy) {
			elements.erase(h);
		} else ++h;
	}
}

void Menu_Screen::Render() {
	Renderer.CameraStop();
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Render();
	}
	Renderer.CameraUpdate();
}

void Menu_Screen::Add(std::unique_ptr<Menu_Element> element) {
	elements.push_back(std::move(element));
}

void Menu_Screen::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) {
	for (auto e = elements.begin(); e != elements.end(); ++e) {
		if (CheckCollision((*e)->GetRect(), mpos)) {
			(*e)->Click(m, button, state, mpos);
		}
	}
}

void Menu::Update() {
	auto scr = GetActiveScreen();
	if (!scr) return;
	scr->Update(this);
}

void Menu::Render() {
	auto scr = GetActiveScreen();
	if (!scr) return;
	scr->Render();
}

void Menu::Click(int button, Keypress_State state, Vec2 mpos) {
	auto scr = GetActiveScreen();
	if (!scr) return;
	scr->Click(this, button, state, mpos);
}

void Menu::AddScreen(const std::string& str, Menu_Screen& screen) {
	screens[str] = std::move(screen);	
}

Menu_Screen* Menu::GetActiveScreen() {
	if (active_menu.empty()) return nullptr;

	auto find = screens.find(active_menu);
	if (find == screens.end())
		return nullptr;
	else
		return &find->second;
}

void CreateCoreMenu(Menu * menu) {
	Menu_Screen main_screen;
	main_screen.Add(std::make_unique<Menu_Elements::Decal>(
		Vec2(0.5,0.2), Vec2(-297.5,-48), Vec2(595,96), std::string("title.png")));

	menu->AddScreen("main", main_screen);

	menu->active_menu = "main";
}

namespace Menu_Elements {

void Decal::Update(Menu * m) { ; }
void Decal::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) { ; }
void Decal::Render() {
	Rect r = GetRect();
	Renderer.RenderTexture(img, nullptr, &r);
}

void Button::Update(Menu * m) {
	Rect r = GetRect();
	if (CheckCollision(r, Vec2(Event.mouse_x, Event.mouse_y))) {
		hovered = 1;
	} else hovered = 0;
}
void Button::Render() {
	Rect r = GetRect();
	if (hovered) {
		Renderer.RenderTexture(img_hover, nullptr, &r);
	} else {
		Renderer.RenderTexture(img, nullptr, &r);
	}
	Renderer.RenderText(font, str, r.x+r.w/2, r.y+r.h/2, font_size, font_color, ALIGN_MIDDLE);
}
void Button::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) {
	if (Function) Function(m);
}

};
