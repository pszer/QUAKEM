#pragma once

#include <vector>
#include <memory>

#include "Vec2.hpp"

struct HUD_Element {
	HUD_Element(Vec2 p, Vec2 off, Vec2 s): pos(p), offset(off), size(s) { ; }

	virtual void Update() = 0;
	virtual void Render() = 0;

	Vec2 pos, offset, size;

	bool destroy = false;

	virtual ~HUD_Element() {}
};

extern struct HUD {
	std::vector<std::unique_ptr<HUD_Element>> elements;

	void Update();
	void Render();
	void Clear();

	void Add(std::unique_ptr<HUD_Element> element);
} HUD;

namespace HUD_Elements {

struct HP_Bar : public HUD_Element {
	HP_Bar(Vec2 p, Vec2 s, int ID):
	  HUD_Element(p,Vec2(0.0,0.0),s), ENT_ID(ID) { ; }

	void Update();
	void Render();

	int ENT_ID;
	double ratio=1.0;
};

struct Ammo_Counter : public HUD_Element {
	Ammo_Counter(Vec2 p, Vec2 off, int ID, int _SLOT, std::string _icon):
		Ammo_Counter(p,Vec2(86.0,64.0),s), ENT_ID(ID), SLOT(_SLOT),
		icon(_icon) { ; }

	void Update();
	void Render();

	int count = 0;

	int ENT_ID, SLOT;
	std::string icon;
};

};
