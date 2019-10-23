#pragma once

#include "Entity.hpp"

namespace Ents {

struct Spawner : public Entity {
	Spawner();

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	std::string Info();

	double rate = 1.0;
	Timer timer;

	Entity_Type type = ENT_ENEMY_WALKER;
	std::vector<Argument> ent_args = {};
	int quantity = 5;
	int counter = 0;

	void Spawn();

	~Spawner() { }
private:
};

}
