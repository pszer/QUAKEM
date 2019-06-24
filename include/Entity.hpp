#pragma once

#include <vector>

#include "Cmd.hpp"
#include "Vec2.hpp"

enum Entity_Team { TEAM_NULL , TEAM_PLAYER , TEAM_ENEMY , TEAM_ENEMY2 , TEAM_OBJECT };

struct Entity {
	Entity() { }

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Hitbox() = 0;
	virtual void Hull() = 0;
	
	// construct from a list of labelled arguments
	// returns 1 for success, 0 for error
	virtual int Construct(const std::vector<Argument>& args);

	Entity_Team team;
	Vec2 pos, vel;
};
