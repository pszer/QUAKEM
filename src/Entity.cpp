#include "Entity.hpp"
#include "Hitreg.hpp"

unsigned long Entity::ID_COUNT = 0;

Rect Entity::Hull() {
	return Rect(pos, size);
}

void Entity::ResetFlags() {
	on_ground = false;
	on_ceiling = false;
	on_leftwall = false;
	on_rightwall = false;
}

void Entity::UpdatePos() {
	pos = pos + (vel * FrameLimit.deltatime);
}
