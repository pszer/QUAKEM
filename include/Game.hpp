#pragma once

#include <vector>
#include <memory>

#include "Hitreg.hpp"
#include "Commands.hpp"
#include "Entity.hpp"
#include "World.hpp"


extern struct Game {
	void Init();
	void Quit();

	std::vector< std::unique_ptr<Entity> > Entities;
	void UpdateEntities();
	void RenderEntities();

	void UpdatePhysics();
	void ApplyGravity(Entity* ent);

	double gravity=0.0, friction=5.0;

	std::map<std::string, Entity_Type> STR_TO_ENT_TYPE;
	// 1 for success, 0 for error
	int CreateEntity(Entity_Type ent, std::vector<Argument>& args);

	bool world_loaded = false;
	struct World World;

	void Update();
	void Render();
} Game;
