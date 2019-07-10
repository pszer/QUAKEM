#pragma once

#include "Entity.hpp"

namespace Ents {

struct Bullet : public Entity {
	Bullet(): Entity(ENT_BULLET,
	  Vec2(0.0,0.0), Vec2(16.0,16.0), Vec2(0.0,0.0), TEAM_NULL, true, false)
	{ timer.Start(); }

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	void BrushCollision(Brush * brush);
	void EntityCollision(Entity * entity);

	std::string Info();

	int damage = 20;
	double lifetime = 0.5;
	Timer timer;

	~Bullet() { }

private:

};

}
