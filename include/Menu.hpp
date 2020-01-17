#pragma once

#include <vector>
#include <map>

#include "Font.hpp"
#include "Vec2.hpp"
#include "Events.hpp"

// pos is relative position on screen (0.0-1.0, 0.0-1.0)
// offset is offset in pixels from pos

typedef struct Menu Menu;
struct Menu_Element {
	Menu_Element(Vec2 p, Vec2 off, Vec2 s):
		pos(p), offset(off), size(s) { ; }

	virtual void Update(Menu * m) = 0;
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

	void Update(Menu * m);
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

		void Update(Menu * m);
		void Render();
		void Click(Menu * m, int button, Keypress_State state, Vec2 mpos); 
	};

	struct Button : public Menu_Element {
		Button(Vec2 p, Vec2 off, Vec2 s,
			const std::string& _img, const std::string& _img_hover,
			const std::string& _str, const std::string& _font, FONT_SIZE f_size, SDL_Color f_col,
			void (*func)(Menu *)):
			Menu_Element(p, off, s), img(_img), img_hover(_img_hover), Function(func),
			str(_str), font(_font), font_size(f_size), font_color(f_col) { ; }
		std::string img, img_hover;
		std::string str, font;
		FONT_SIZE font_size;
		SDL_Color font_color;
		void (*Function)(Menu * m);

		int hovered=0;

		void Update(Menu * m);
		void Render();
		void Click(Menu * m, int button, Keypress_State state, Vec2 mpos);
	};
};
