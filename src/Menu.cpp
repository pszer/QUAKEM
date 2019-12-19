#include "Menu.hpp"

struct Menu Menu;

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

void Menu::Update() {

}

void Menu::Render() {

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
