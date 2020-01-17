#pragma once

#include <vector>
#include <map>

#include "Vec2.hpp"
#include "Events.hpp"

// pos is relative position on screen (0.0-1.0, 0.0-1.0)
// offset is offset in pixels from pos

typedef struct Menu Menu;
struct Menu_Element {
	Menu_Element(Vec2 p, Vec2 off, Vec2 s):
		pos(p), offset(off), size(s) { ; }

	virtual void Update() = 0;
	virtual void Render() = 0;
	// Click() called when clicked by user.
	//                 MOUSE1/2/3  KEY_DOWN/HELD/UP
	virtual void Click(Menu * m, int button, Keypress_State state, Vec2 mpos) { ; }

	Vec2 pos, offset, size;
	Rect GetRect();

	bool destroy = false;

	virtual ~Menu_Element() {}
};

struct Menu_Screen {
	std::vector<std::unique_ptr<Menu_Element>> elements;

	void Update();
	void Render();
	void Click(Menu * m, int button, Keypress_State state, Vec2 mpos);

	void Add(std::unique_ptr<Menu_Element> element);
};

struct Menu {
	std::string active_menu = "";
	std::map<std::string, Menu_Screen> screens;

	void Update();
	void Render();
	void Click(int button, Keypress_State state, Vec2 mpos);

	void AddScreen(const std::string& str, Menu_Screen& screen);

	// returns nullptr if no active_menu or
	// 'active_menu' menu doesn't exist
	Menu_Screen* GetActiveScreen();
};

void CreateCoreMenu(Menu * menu);

namespace Menu_Elements {
	struct Decal : public Menu_Element {
		Decal(Vec2 p, Vec2 off, Vec2 s, const std::string& _img):
			Menu_Element(p, off, s), img(_img) { ; }
		std::string img;

		void Update();
		void Render();
		void Click(Menu * m, int button, Keypress_State state, Vec2 mpos); 
	};

	struct ImgButton : public Menu_Element {
		ImgButton(Vec2 p, Vec2 off, Vec2 s, const std::string& _img, std::string& _img_hover,
			void (*func)(Menu *)):
			Menu_Element(p, off, s), img(_img), img_hover(_img_hover), Function(func) { ; }
		std::string img, img_hover;
		void (*Function)(Menu * m);

		int hovered=0;

		void Update();
		void Render();
		void Click(Menu * m, int button, Keypress_State state, Vec2 mpos) { ; }
	};

	struct TextButton : public Menu_Element {
		TextButton(Vec2 p, Vec2 off, Vec2 s, const std::string& _str, void (*func)(Menu *)):
			Menu_Element(p, off, s), str(_str), Function(func) { ; }
		std::string str;
		void (*Function)(Menu * m);

		void Update();
		void Render();
		void Click(Menu * m, int button, Keypress_State state, Vec2 mpos) { ; }
	};
};
