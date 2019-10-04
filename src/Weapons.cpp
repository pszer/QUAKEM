#include "Weapons.hpp"
#include "Game.hpp"

namespace Weapons {

std::unique_ptr<Weapon> CreateWeapon(WEAPON_TYPE type, std::map<std::string, double> keys, Entity* p) {
	switch (type) {
	case WEP_PISTOL: return std::make_unique<Pistol>(keys, p);
	}

	return nullptr;
}

bool Pistol::Fire(Vec2 aim) {
	if (timer.GetSeconds() < GetKey("rate")) return false;
	timer.Reset();

	double length = std::sqrt(aim.x*aim.x + aim.y*aim.y);
	if (length == 0.0) return false;
	aim = aim / length;


	double vel = GetKey("vel");

	aim = aim * vel;

	std::vector<Argument> args = {
	  Argument(aim.x, "xv"),
	  Argument(aim.y, "yv"),
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

