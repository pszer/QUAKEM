#pragma once

#include <map>
#include <vector>

#include "Entity.hpp"
#include "Vec2.hpp"

// BRUSH_SOLID's block all entitites
// BRUSH_NONSOLID's don't block entities
// BRUSH_BACKGROUND's don't block entities and they are drawn in the background
// BRUSH_FOREGROUND's don't block entities and they are drawn in the foreground
//
// entity collisions with non BRUSH_SOLIDS are checked for nethertheless if collide_func != nullptr
// update() is called every frame if it is != nullptr
//
// std::map keys is used to specify properties for special brushes like push brushes
enum BRUSH_TYPE { BRUSH_SOLID , BRUSH_NONSOLID , BRUSH_BACKGROUND , BRUSH_FOREGROUND };

struct Brush {
	Rect rect;
	BRUSH_TYPE type;

	std::string texture = "";
	std::map<std::string, int> keys;

	void Render();

	void (*collide_func)(Entity * ent) = nullptr;
	void (*update)() = nullptr;
};

struct World {
	void Update();
	void CollideWithEntities();
	void Clear();

	void RenderBackground();
	void RenderMiddle();
	void RenderForeground();

	std::vector<Brush> brushes;
};
