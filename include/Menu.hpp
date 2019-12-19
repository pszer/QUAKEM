#pragma once

#include <vector>
#include <map>

#include "Vec2.hpp"
#include "Events.hpp"

struct Menu_Element {
	Menu_Element(Vec2 p, Vec2 off, Vec2 s):
		pos(p), offset(off), size(s) { ; }

	virtual void Update() = 0;
	virtual void Render() = 0;
	// Click() called when clicked by user.
	//                 MOUSE1/2/3  KEY_DOWN/HELD/UP
	virtual void Click(int button, Keypress_State state) { ; }

	Vec2 pos, offset, size;

	bool destroy = false;

	virtual ~Menu_Element() {}
};

struct Menu_Screen {
	std::vector<std::unique_ptr<Menu_Element>> elements;

	void Update();
	void Render();

	void Add(std::unique_ptr<Menu_Element> element);
};

extern struct Menu {
	std::string active_menu = "";
	std::map<std::string, Menu_Screen> screens;

	void Update();
	void Render();

	void AddScreen(const std::string& str, Menu_Screen& screen);

	// returns nullptr if no active_menu or
	// 'active_menu' menu doesn't exist
	Menu_Screen* GetActiveScreen();
} Menu;

namespace Menu_Elements {

};
