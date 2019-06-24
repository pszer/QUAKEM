#include "Game.hpp"
#include "EntList.hpp"

struct Game Game;

void Game::Update() {
	UpdateEntities();
}

void Game::Render() {
	RenderEntities();
}

void Game::UpdateEntities() {
	for (auto ent = Entities.begin(); ent != Entities.end();) {
		if (!(*ent)->destroy) {
			(*ent)->Update();
			++ent;
		} else {
			Entities.erase(ent);
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
