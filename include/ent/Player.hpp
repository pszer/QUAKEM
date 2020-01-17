#pragma once

#include "Entity.hpp"
#include "Weapons.hpp"

namespace Ents {

struct Player : public Entity {
	Player(): Entity(ENT_PLAYER,
	  Vec2(0.0,0.0), Vec2(25.0,50.0), Vec2(0.0,0.0), TEAM_PLAYER)
	{ 
		hitpoints = 100; max_hitpoints = 100;

		AddWeapon(WEP_PISTOL, {
				{"ammo",-1}, {"dmg", 15.0}, {"rate", 0.12}, {"vel", 2500.0}, {"life", 0.35}, {"spread",0.0} }); 
		AddWeapon(WEP_PISTOL, {
		  {"ammo",100}, {"dmg", 7.0}, {"rate", 0.05}, {"vel", 1800.0}, {"life", 0.25}, {"spread",0.2},
		  {"autofire", 1.0} });
		AddWeapon(WEP_ROCKETL, {
		  {"ammo",5}, {"dmg", 65.0}, {"rate", 0.42 }, {"vel", 1000.0 }, {"rad", 100.0} });
		AddWeapon(WEP_GRENADEL, {
		  {"ammo",3}, {"dmg", 40.0}, {"rate", 0.42 }, {"vel", 1000.0 }, {"rad", 100.0}, {"burst", 3.0} });
	}

	void Update();
	void Render();
	Rect Hitbox();

	void HandleInput();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	std::string Info();

	bool move_left = false, move_right = true;

	~Player() { }

private:
	void MoveLeft();
	void StopMoveLeft();
	void MoveRight();
	void StopMoveRight();
	void Jump();
	void Fire();
};

}
