#pragma once

#include "World.hpp"

namespace Brushes {

// keys
// xforce - push in x direction
// yforce - push in y direction
struct Push : public Brush {
	Push(Rect _r, const std::string& tex="", Vec2 scale = Vec2(1.0,1.0),
	  Vec2 offset = Vec2(0.0,0.0), std::map<std::string, double> _k=std::map<std::string, double>())
	: Brush(_r, BRUSH_PUSH, tex, scale, offset, std::move(_k)) 
	{
		check_collision = true;
	}

	void CollideFunc(Entity * ent);
};

}
