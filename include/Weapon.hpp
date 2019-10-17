#pragma once

#include <cmath>

#include "Entity.hpp"

class Weapon {
public:
	Weapon(std::string _name, std::string _icon): name(_name), icon(_icon) { ; }

	virtual void Fire(double angle) { ; }
	virtual void Fire(Vec2 aim) { Fire(std::atan2(aim.y, aim.x)); }
private:
	std::string name, icon;
};
