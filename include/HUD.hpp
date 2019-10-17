#pragma once

#include <vector>
#include <memory>

#include "Vec2.hpp"

struct HUD_Element {
	HUD_Element(Vec2 p, Vec2 s): pos(p), size(s) { ; }

	virtual void Update() = 0;
	virtual void Render() = 0;

	Vec2 pos, size;

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
	  HUD_Element(p,s), ENT_ID(ID) { ; }

	void Update();
	void Render();

	int ENT_ID;
	double ratio=1.0;
};

};
