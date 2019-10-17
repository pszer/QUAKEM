#pragma once

#include "Entity.hpp"

namespace Ents {

struct Bullet : public Entity {
	Bullet();

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	void BrushCollision(Brush * brush);
	void EntityCollision(Entity * entity);

	std::string Info();

	int damage = 20;
	double lifetime = 0.5;
	Timer timer;

	~Bullet() { }

private:

};

struct Rocket : public Entity {
	Rocket();

	void Update();
	void Render();
	Rect Hitbox();
	Rect Hull();

	static const std::string CONSTRUCT_MSG;
	int Construct(const std::vector<Argument>& args);

	void BrushCollision(Brush * brush);
	void EntityCollision(Entity * entity);

	void SpawnExplosion();

	std::string Info();

	int damage = 20;
	double radius = 50.0;
	
	~Rocket() { }
};

}
