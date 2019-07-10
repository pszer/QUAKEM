#pragma once

#include "Entity.hpp"

namespace Ents {

struct Player : public Entity {
	Player(): Entity(ENT_PLAYER,
	  Vec2(0.0,0.0), Vec2(25.0,50.0), Vec2(0.0,0.0), TEAM_PLAYER)
	{ 
		hitpoints = 100; max_hitpoints = 100;
		cooldown.Start();
	}

	void Update();
	void Render();
	Rect Hitbox();

	void HandleInput();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	std::string Info();

	int shot_counter = 0, shot_burst = 4;
	double fire_cooldown = 0.5, shot_delay = 0.048;
	bool shot = false;
	Timer cooldown, burst_timer;

	bool move_left = false, move_right = true;

	~Player() { }

private:
	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();
	void Jump();
	void Fire();
	void SpawnBullets();
};

}
