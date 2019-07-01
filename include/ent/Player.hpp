#pragma once

#include "Entity.hpp"

namespace Ents {

struct Player : public Entity {
	Player(): Entity(Vec2(0.0,0.0), Vec2(25.0,50.0), Vec2(0.0,0.0), TEAM_PLAYER) { }

	void Update();
	void Render();
	Rect Hitbox();

	void HandleInput();

	const std::string CONSTRUCT_USE = "x y w h hp";
	int Construct(const std::vector<Argument>& args);

	int hitpoints=100;

	bool move_left = false, move_right = true;

	~Player() { }

private:
	void MoveLeft();
	void MoveRight();
	void Jump();
};

}
