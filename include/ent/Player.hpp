#pragma once

#include "Entity.hpp"

namespace Ents {

struct Player : public Entity {
	Player(): team(TEAM_PLAYER), vel(0.0,0.0) { }

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	void HandleInput();

	const std::string CONSTRUCT_USE = "x y w h hp";
	int Construct(const std::vector<Argument>& args);

	int hitpoints=100;
	Entity_Team team;

	Vec2 pos,size=Vec2(25,50),vel;

	~Player() { }

private:
	void MoveLeft();
	void MoveRight();
	void Jump();
};

}
