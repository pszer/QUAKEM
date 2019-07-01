#pragma once

#include <vector>

#include "Render.hpp"
#include "Cmd.hpp"
#include "Vec2.hpp"
#include "Timer.hpp"
#include "Events.hpp"
#include "Keys.hpp"

enum Entity_Team { TEAM_NULL , TEAM_PLAYER , TEAM_ENEMY , TEAM_ENEMY2 , TEAM_OBJECT };

struct Entity {
	Entity(Vec2 _pos=Vec2(0.0,0.0), Vec2 _size=Vec2(0.0,0.0),
	       Vec2 _vel=Vec2(0.0,0.0), Entity_Team _team=TEAM_NULL)
	: pos(_pos), size(_size), vel(_vel), team(_team) { }

	virtual void Update() = 0;
	virtual void Render() { }
	virtual Rect Hitbox() { return Rect(0.0,0.0,0.0,0.0); }
	Rect Hull(); 
	
	// construct from a list of labelled arguments
	// returns 1 for success, 0 for error
	const std::string CONSTRUCT_USE;
	virtual int Construct(const std::vector<Argument>& args) = 0;

	Entity_Team team = TEAM_NULL;
	Vec2 pos, size, vel;

	void UpdatePos();

	// entity will be destroyed in the UpdateEntities() function
	bool destroy = false;

	void ResetFlags();
	bool on_ground = false,
	     on_ceiling = false,
	     on_leftwall = false,
	     on_rightwall = false;

	virtual ~Entity() { }
};
