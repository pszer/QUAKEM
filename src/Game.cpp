#include "Game.hpp"
#include "EntList.hpp"

struct Game Game;

void Game::Update() {
	UpdatePhysics();
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
	}
}

void Game::UpdatePhysics() {
	gravity = GetCVarFloat("gravity");
	friction = GetCVarFloat("friction");
	for (auto ent = Entities.begin(); ent != Entities.end(); ++ent) {
		(*ent)->ResetFlags();
		if ((*ent)->collide) {
			for (auto b = World.Brushes.begin(); b != World.Brushes.end(); ++b) {
				bool collide = (*b)->type == BRUSH_SOLID;
				EntityRectCollision(ent->get(), (*b)->rect, collide);
			}
		}

		(*ent)->UpdatePos();

		if ((*ent)->physics) {
			ApplyGravity(ent->get());
			(*ent)->vel.x /= 1.0 + (friction * FrameLimit.deltatime);
		}
	}
}

void Game::ApplyGravity(Entity* ent) {
	ent->vel.y += gravity * FrameLimit.deltatime;
}

void Game::RenderEntities() {
	for (auto ent = Entities.begin(); ent != Entities.end(); ++ent) {
		(*ent)->Render();
	}
}

void Game::Init() {
	STR_TO_ENT_TYPE["ENT_PLAYER"] = ENT_PLAYER;

	CVARS["gravity"] = Argument(600.0);
	CVARS["friction"] = Argument(25.0);
	CVARS["player_speed"] = Argument(300.0);
	CVARS["player_jump"] = Argument(350.0);

	World.Brushes.push_back(std::make_unique<Brush>(
	  Rect(112.0,384.0,576.0,192.0), BRUSH_SOLID, "img/64.png", Vec2(1.0,1.0), Vec2(112.0,0.0)));
	World.Brushes.push_back(std::make_unique<Brush>(
	  Rect(368.0,256.0,128.0,64.0), BRUSH_SOLID, "img/64.png", Vec2(1.0,1.0), Vec2(112.0,0.0)));
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
