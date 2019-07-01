#include "Game.hpp"
#include "EntList.hpp"

struct Game Game;

void Game::Update() {
	UpdateEntities();

	World.CollideWithEntities();
	World.Update();
}

void Game::Render() {
	World.RenderBackground();
	World.RenderMiddle();
	RenderEntities();
	World.RenderForeground();
}

void Game::UpdateEntities() {
	for (auto ent = Entities.begin(); ent != Entities.end();) {
		if (!(*ent)->destroy) {
			(*ent)->Update();
			++ent;
		} else {
			Entities.erase(ent);
		}

		for (auto b = World.Brushes.begin(); b != World.Brushes.end(); ++b) {
			//if (EntityRectCollision(*ent, (*b)->rect, true)) break;	
		}
	}
}

void Game::RenderEntities() {
	for (auto ent = Entities.begin(); ent != Entities.end(); ++ent) {
		(*ent)->Render();
	}
}

void Game::Init() {
	STR_TO_ENT_TYPE["ENT_PLAYER"] = ENT_PLAYER;

	World.Brushes.push_back(std::make_unique<Brush>(
	  //Rect(64.0,384.0,672.0,128.0), BRUSH_SOLID, "img/dev.jpg", Vec2(1.0,1.0)));
	  Rect(112.0,384.0,576.0,192.0), BRUSH_SOLID, "img/64.png", Vec2(1.0,1.0), Vec2(112.0,0.0)));
}

void Game::Quit() {
	World.Clear();
}

int Game::CreateEntity(ENT_TYPE ent_type, std::vector<Argument>& args) {
	std::unique_ptr<Entity> ent;
	switch (ent_type) {
	case ENT_PLAYER: ent = std::make_unique<Ents::Player>(); break;
	}

	if (!ent->Construct(args)) return 0;
	Entities.push_back(std::move(ent));
	return 1;
}
