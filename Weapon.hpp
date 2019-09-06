#pragma once

#include <cmath>

#include "Entity.hpp"

enum WEAPON_TYPE { };

class Weapon {
public:
	Weapon(std::string n): name(n) { }

	virtual bool Fire(double angle) { return false; }
	bool Fire(Vec2 aim) { Fire(std::atan2(aim.x,aim.y)); }

	std::string name = "";
};
