#pragma once

#include <memory>

#include "Weapon_Class.hpp"

namespace Weapons {

std::unique_ptr<Weapon> CreateWeapon(WEAPON_TYPE type, std::map<std::string, double>, Entity* p);

// WEP_PISTOL
// KEY    / DESCRIPTION
//  dmg   -  damage per bullet
//  rate  -  cooldown between shots
//  vel   -  bullet velocity
//  life  -  bullet lifetime
class Pistol : public Weapon {
public:
	Pistol(std::map<std::string, double> k, Entity * p):
	 Weapon(k,p) { timer.Start(); }
	bool Fire(double angle);

	Timer timer;
};

};

