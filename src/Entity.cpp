#include "Entity.hpp"
#include "Hitreg.hpp"

unsigned long Entity::ID_COUNT = 0;

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

void Entity::AddWeapon(WEAPON_TYPE type, std::map<std::string, double> keys, int slot) {
	auto wep = Weapons::CreateWeapon(type, keys, this);

	if (slot == -1)
		weapons.push_back(std::move(wep));
	else
		weapons.insert(weapons.begin() + slot, std::move(wep));
}

void Entity::SwitchWeapon(int slot) {
	if (weapons.size() >= slot) return;
	if (weapons.at(slot) == nullptr) return;
	active_weapon = slot;
}
