#pragma once

#include "Entity.hpp"

namespace Ents {

class Medpack : public Entity {
public:
	Medpack(): Entity(ENT_MEDPACK, Vec2(0.0,0.0), Vec2(32.0,32.0)) { }

	void Update();
	void Render();
	Rect Hitbox() { return Hull(); }

	void EntityCollision(Entity * entity);
	
	// construct from a list of labelled arguments
	// returns 1 for success, 0 for error
	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	int heal = 50;

	// constructs a string about entity status
	std::string Info();
private:
};

}
