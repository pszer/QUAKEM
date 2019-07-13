#pragma once

#include <utility>

#include "Entity.hpp"

namespace Brushes {

// keys
// damage - damage per tick
// rate   - how fast to deal damage (tick length)
struct Hurt : public Brush {
	Hurt(Rect _r, const std::string& tex="", Vec2 scale = Vec2(1.0,1.0),
	  Vec2 offset = Vec2(0.0,0.0), std::map<std::string, double> _k=std::map<std::string, double>())
	: Brush(_r, BRUSH_BACKGROUND, tex, scale, offset, std::move(_k)) 
	{
		check_collision = true;
	}

	void Update();
	void CollideFunc(Entity * ent);
	std::vector<std::pair<int, Timer>> timers;
};

}
