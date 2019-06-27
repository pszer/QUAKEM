#pragma once

#include <vector>
#include <memory>

#include "Commands.hpp"
#include "Entity.hpp"
#include "World.hpp"

enum ENT_TYPE { ENT_PLAYER };

extern struct Game {
	void Init();
	void Quit();

	std::vector< std::unique_ptr<Entity> > Entities;
	void UpdateEntities();
	void RenderEntities();

	std::map<std::string, ENT_TYPE> STR_TO_ENT_TYPE;
	// 1 for success, 0 for error
	int CreateEntity(ENT_TYPE ent, std::vector<Argument>& args);

	bool world_loaded = false;
	struct World World;

	void Update();
	void Render();
} Game;
