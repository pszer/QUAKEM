#include "Weapons.hpp"
#include "Game.hpp"

namespace Weapons {

std::unique_ptr<Weapon> CreateWeapon(WEAPON_TYPE type, std::map<std::string, double> keys, Entity* p) {
	switch (type) {
	case WEP_PISTOL:  return std::make_unique<Pistol>(keys, p);
	case WEP_ROCKETL: return std::make_unique<RocketLauncher>(keys, p);
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
	double spread = GetKey("spread");

	double rand;
	rand = spread * ((std::rand()%101)/100.0-0.5);
	double COS = std::cos(rand);
	double SIN = std::sin(rand);

	Vec2 new_aim;
	new_aim.x = COS * aim.x - SIN * aim.y;
	new_aim.y = SIN * aim.x + COS * aim.y;	

	new_aim = new_aim * vel;

	std::string team = TeamToStr(parent->team);

	std::vector<Argument> args = {
	  Argument(new_aim.x, "xv"),
	  Argument(new_aim.y, "yv"),
	  Argument(parent->Hull().Middle().x, "x"),
	  Argument(parent->Hull().Middle().y, "y"),
	  Argument(GetKey("dmg"), "dmg"),
	  Argument(GetKey("life"), "life"),
	  Argument(ARG_STRING, team, "team"),
	};

	Game.CreateEntity(ENT_BULLET, args);

	return true;
}

bool RocketLauncher::Fire(Vec2 aim) {
	if (timer.GetSeconds() < GetKey("rate")) return false;
	timer.Reset();

	double length = std::sqrt(aim.x*aim.x + aim.y*aim.y);
	if (length == 0.0) return false;
	aim = aim / length;

	double vel = GetKey("vel");

	aim = aim * vel;

	std::string team = TeamToStr(parent->team);

	std::vector<Argument> args = {
		Argument(aim.x, "xv"),
		Argument(aim.y, "yv"),
		Argument(parent->Hull().Middle().x, "x"),
		Argument(parent->Hull().Middle().y, "y"),
	  	Argument(GetKey("dmg"), "dmg"),
		Argument(GetKey("rad"), "rad"),
	  	Argument(ARG_STRING, team, "team")
	};

	Game.CreateEntity(ENT_ROCKET, args);

	return true;
}

};

