#include "Weapons.hpp"
#include "Game.hpp"

bool Weapon::Fire(Vec2 aim) {
	  return Fire(std::atan2(aim.y - parent->Hull().Middle().y,
	                         aim.x - parent->Hull().Middle().x ));
}

namespace Weapons {

std::unique_ptr<Weapon> CreateWeapon(WEAPON_TYPE type, std::map<std::string, double> keys, Entity* p) {
	switch (type) {
	case WEP_PISTOL: return std::make_unique<Pistol>(keys, p);
	}

	return nullptr;
}

bool Pistol::Fire(double angle) {
	if (timer.GetSeconds() < GetKey("rate")) return false;
	timer.Reset();

	double vx = std::cos(angle);
	double vy = std::sin(angle);

	double vel = GetKey("vel");

	vx *= vel;
	vy *= vel;

	std::vector<Argument> args = {
	  Argument(vx, "xv"),
	  Argument(vy, "yv"),
	  Argument(parent->Hull().Middle().x, "x"),
	  Argument(parent->Hull().Middle().y, "y"),
	  Argument(GetKey("dmg"), "dmg"),
	  Argument(GetKey("life"), "life"),
	  Argument(ARG_STRING, "player", "team"),
	};

	Game.CreateEntity(ENT_BULLET, args);

	return true;
}

};

