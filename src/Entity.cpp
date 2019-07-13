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
