#include "Menu.hpp"
#include "Events.hpp"
#include "Hitreg.hpp"

struct Menu Menu;

Rect Menu_Element::GetRect() {
	Rect result = Rect(0.0, 0.0, size.x, size.y);

	result.x = pos.x * Event.win_w + offset.x;
	result.y = pos.y * Event.win_h + offset.y;

	return result;
}

void Menu_Screen::Update() {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Update();
	}

	for (auto h = elements.begin(); h != elements.end();) {
		if ((*h)->destroy) {
			elements.erase(h);
		} else ++h;
	}
}

void Menu_Screen::Render() {
	for (auto h = elements.begin(); h != elements.end(); ++h) {
		(*h)->Render();
	}
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
	scr->Update();
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

}

namespace Menu_Elements {

void Decal::Update();
void Decal::Render();
void Decal::Click(Menu * m, int button, Keypress_State state, Vec2 mpos) { ; }

};
