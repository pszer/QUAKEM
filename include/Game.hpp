#pragma once

#include <vector>
#include <memory>

#include "Commands.hpp"
#include "Entity.hpp"

extern struct Game {
	std::vector< std::unique_ptr<Entity> > ents;
	void UpdateEntities();
	void RenderEntities();
} Game;
