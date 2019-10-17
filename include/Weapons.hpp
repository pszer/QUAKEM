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
	bool Fire(Vec2 aim);

	Timer timer;
};

// WEP_ROCKETL
// KEY    / DESCRIPTION
// dmg    - damage
// vel    - rocket velocity
// rad    - explosion radius
// rate   - cooldown between shots
class RocketLauncher : public Weapon {
public:
	RocketLauncher(std::map<std::string, double> k, Entity * p):
	 Weapon(k,p) { timer.Start(); }
	bool Fire(Vec2 aim);

	Timer timer;
};

// WEP_MACHINEGUN
// KEY    / DESCRIPTION
//  dmg   -  damage per bullet
//  rate  -  cooldown between shots
//  vel   -  bullet velocity
//  life  -  bullet lifetime/
/*class MachineGun : public Weapon {

};*/

};

