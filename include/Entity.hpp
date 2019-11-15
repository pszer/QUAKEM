#pragma once

#include <vector>
#include <utility>

#include "Render.hpp"
#include "Cmd.hpp"
#include "Vec2.hpp"
#include "Timer.hpp"
#include "Events.hpp"
#include "Keys.hpp"
#include "World.hpp"
#include "Weapons.hpp"

enum Entity_Type { ENT_NULL , ENT_PLAYER , ENT_MEDPACK , ENT_BULLET , ENT_ROCKET ,
	ENT_GRENADE , ENT_EXPLOSION , ENT_SPAWNER , ENT_ENEMY_WALKER };
enum Entity_Team { TEAM_NULL , TEAM_PLAYER , TEAM_ENEMY , TEAM_OBJECT };

std::string TeamToStr(Entity_Team);
Entity_Team StrToTeam(const std::string&);

std::string EntToStr(Entity_Type);
Entity_Type StrToEnt(const std::string&);

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
	virtual void HandleInput() { }
	virtual Rect Hitbox() { return Rect(0.0,0.0,0.0,0.0); }
	virtual Rect Hull() { return Rect(pos, size); }

	virtual void BrushCollision(Brush * brush) { }
	virtual void EntityCollision(Entity * entity) { }
	
	// construct from a list of labelled arguments
	// returns 1 for success, 0 for error
	virtual int Construct(const std::vector<Argument>& args) = 0;

	// constructs a string about entity status
	virtual std::string Info()
	  { return "x:" + std::to_string(pos.x) + " y:" + std::to_string(pos.y); }

	Entity_Type type;
	Entity_Team team = TEAM_NULL;
	Vec2 pos, size, vel;

	void Heal(int amount);
	int hitpoints=1, max_hitpoints=1;

	void UpdatePos();
	void Push(Vec2 push);

	bool collide = true, physics = true;
	double bounce = 0.0;

	// entity will be destroyed in the UpdateEntities() function
	bool destroy = false;

	void ResetFlags();
	bool on_ground = false,
	     on_ceiling = false,
	     on_leftwall = false,
	     on_rightwall = false;

	int active_weapon = 0;
	std::vector<std::unique_ptr<Weapon>> weapons;
	Weapon * ActiveWeapon() {
		return active_weapon < weapons.size()
		  ? weapons[active_weapon].get() : nullptr; }
	void SwitchWeapon(int slot);
	Weapon * GetWeapon(int slot);

	void AddWeapon(WEAPON_TYPE type, std::map<std::string, double> keys, int slot=-1);

	virtual ~Entity() { }
};

// sound effect used for ent positional sound
void _ent_pos_update(int id);
