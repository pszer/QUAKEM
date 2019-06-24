#pragma once

#include "Entity.hpp"

namespace Ents {

struct Player : public Entity {
	Player(): team(TEAM_PLAYER), vel(0.0,0.0) { }

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	const std::string CONSTRUCT_USE = "x y w h hp";
	int Construct(const std::vector<Argument>& args);

	int hitpoints;
	Entity_Team team;

	Vec2 pos,size,vel;

	~Player() { }
};

}
