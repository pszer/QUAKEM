#pragma once

#include <memory>
#include <cstdlib>

enum WEAPON_TYPE { WEP_PISTOL , WEP_ROCKETL , WEP_GRENADEL };

#include "Weapon_Class.hpp"

namespace Weapons {

extern std::string SLOT_TO_ICON[];

std::unique_ptr<Weapon> CreateWeapon(WEAPON_TYPE type, std::map<std::string, double>, Entity* p);

// WEP_PISTOL
// KEY     / DESCRIPTION
//  dmg    -  damage per bullet
//  rate   -  cooldown between shots
//  vel    -  bullet velocity
//  life   -  bullet lifetime
//  spread -  random spread (in radians)
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

// WEP_GRENADEL
// KEY     / DESCRIPTION
// dmg     - dmg
// vel     - grenade velocity
// rad     - explosion radius
// burst   - grenade burst count
class GrenadeLauncher : public Weapon {
public:
	GrenadeLauncher(std::map<std::string, double> k, Entity * p):
	  Weapon(k,p) { timer.Start(); }
	bool Fire(Vec2 aim);

	Timer timer;

};

};

