#pragma once

#include <vector>

#include "Vec2.hpp"

struct HUD {
	std::vector<std::unique_ptr<HUD_Element>> elements;

	void Update();
	void Render();
	void Clear();

	template <typename T>
	void Add() {

	}
};

struct HUD_Element {
	HUD_Element(Vec2 p, Vec2 s): pos(p), size(s) { ; }

	virtual void Update() = 0;
	virtual void Render() = 0;

	Vec2 pos, size;
};
