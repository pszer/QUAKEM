#pragma once

#include "Entity.hpp"

namespace Ents {

struct Explosion : public Entity {
	Explosion();

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	void EntityCollision(Entity * entity);

	std::string Info();

	bool lived = false;

	int damage = 20;
	double radius = 100.0;

	~Explosion() { }
private:

};

}
