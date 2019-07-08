#pragma once

#include <vector>

#include "Render.hpp"
#include "Cmd.hpp"
#include "Vec2.hpp"
#include "Timer.hpp"
#include "Events.hpp"
#include "Keys.hpp"

enum Entity_Type { ENT_NULL , ENT_PLAYER };
enum Entity_Team { TEAM_NULL , TEAM_PLAYER , TEAM_ENEMY , TEAM_ENEMY2 , TEAM_OBJECT };

struct Entity {
	Entity(Entity_Type _type, Vec2 _pos=Vec2(0.0,0.0), Vec2 _size=Vec2(0.0,0.0),
	       Vec2 _vel=Vec2(0.0,0.0), Entity_Team _team=TEAM_NULL,
	       bool _collide=true, bool _physics = true)
	: type(_type), pos(_pos), size(_size), vel(_vel), team(_team),
	  collide(_collide), physics(_physics),
	  UNIQUE_ID(ID_COUNT++) // assign unique id
	  { }

	static unsigned long ID_COUNT;
	const int UNIQUE_ID;

	virtual void Update() = 0;
	virtual void Render() { }
	virtual Rect Hitbox() { return Rect(0.0,0.0,0.0,0.0); }
	Rect Hull(); 
	
	// construct from a list of labelled arguments
	// returns 1 for success, 0 for error
	const std::string CONSTRUCT_USE;
	virtual int Construct(const std::vector<Argument>& args) = 0;

	// constructs a string about entity status
	virtual std::string Info()
	  { return "x:" + std::to_string(pos.x) + " y:" + std::to_string(pos.y); }

	Entity_Type type;
	Entity_Team team = TEAM_NULL;
	Vec2 pos, size, vel;

	void UpdatePos();

	bool collide = true, physics = true;

	// entity will be destroyed in the UpdateEntities() function
	bool destroy = false;

	void ResetFlags();
	bool on_ground = false,
	     on_ceiling = false,
	     on_leftwall = false,
	     on_rightwall = false;

	virtual ~Entity() { }
};
