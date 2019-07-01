#include "Entity.hpp"
#include "Hitreg.hpp"

Rect Entity::Hull() {
	return Rect(pos, size);
}

void Entity::ResetFlags() {
	on_ground = false;
	on_ceiling = false;
	on_leftwall = false;
	on_rightwall = false;
}
