#include "Entity.hpp"
#include "Hitreg.hpp"
#include "Game.hpp"

unsigned long Entity::ID_COUNT = 0;

const std::pair<std::string, Entity_Team> __TEAMSTRS[4] =
  {{"NULL",TEAM_NULL},{"PLAYER",TEAM_PLAYER},
   {"ENEMY",TEAM_ENEMY},{"OBJECT",TEAM_OBJECT}};
std::string TeamToStr(Entity_Team t) {
	for (int i = 0; i < 4; ++i)
		if (__TEAMSTRS[i].second == t) return __TEAMSTRS[i].first;
	return "NULL";
}

Entity_Team StrToTeam(const std::string& str) {
	for (int i = 0; i < 4; ++i)
		if (__TEAMSTRS[i].first == str) return __TEAMSTRS[i].second;
	return TEAM_NULL;
}

std::string EntToStr(Entity_Type t) {
	for (auto& p : Game.STR_TO_ENT_TYPE) {
		if (p.second == t) return p.first;
	}
	return "ENT_NULL";
}

Entity_Type StrToEnt(const std::string& str) {
	auto f = Game.STR_TO_ENT_TYPE.find(str);
	if (f == Game.STR_TO_ENT_TYPE.end()) return ENT_NULL;
	return f->second;
}

void Entity::ResetFlags() {
	on_ground = false;
	on_ceiling = false;
	on_leftwall = false;
	on_rightwall = false;
}


void Entity::Heal(int amount) {
	hitpoints += amount;
	if (hitpoints > max_hitpoints) hitpoints = max_hitpoints;
}

void Entity::UpdatePos() {
	pos = pos + (vel * FrameLimit.deltatime);
}

void Entity::Push(Vec2 push) {
	vel = vel + push;
}

void Entity::AddWeapon(WEAPON_TYPE type, std::map<std::string, double> keys, int slot) {
	auto wep = Weapons::CreateWeapon(type, keys, this);

	if (slot == -1)
		weapons.push_back(std::move(wep));
	else
		weapons.insert(weapons.begin() + slot, std::move(wep));
}

Weapon * Entity::GetWeapon(int slot) {
	if (slot >= -1 && slot < weapons.size() && weapons.at(slot) != nullptr)
		return weapons.at(slot).get();
	return nullptr;
	
}

void Entity::SwitchWeapon(int slot) {
	if (slot >= -1 && slot < weapons.size() && weapons.at(slot) != nullptr)
	active_weapon = slot;
}
