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
	Renderer.CameraUpdate();
	World.RenderBackground();
	World.RenderMiddle();
	RenderEntities();
	World.RenderForeground();
}

void Game::CameraUpdate() {
	switch (camera_mode) {
	case GAME_CAMERA_DEFAULT:
		CameraDefault();
		break;
	case GAME_CAMERA_STATIC:
		CameraStatic(camera_pos, camera_zoom);
		break;
	case GAME_CAMERA_FOLLOW_PLAYER:
		CameraFollowPlayer(camera_zoom);
		break;
	case GAME_CAMERA_FOLLOW_ENT:
		CameraFollowEntity(camera_id, camera_zoom);
		break;
	case GAME_CAMERA_PATH:
		CameraPath(camera_start, camera_end, camera_duration, camera_zoom);
		break;
	}
}

void Game::CameraDefault() {
	Renderer.camera = nullptr;
}

void Game::CameraFollowEntity(unsigned long ID, double zoom) {
	camera = Camera(ID, zoom);
	Renderer.camera = &camera;
}

void Game::CameraFollowPlayer(double zoom) {
	for (auto ent = Entities.begin(); ent != Entities.end(); ++ent) {
		if ((*ent)->type == ENT_PLAYER) {
			CameraFollowEntity((*ent)->UNIQUE_ID, zoom);
			return;
		}
	}
	CameraDefault();
}

void Game::CameraStatic(Vec2 pos, double zoom) {
	camera = Camera(pos, zoom);	
	Renderer.camera = &camera;
}

void Game::CameraPath(Vec2 start, Vec2 end, double duration, double zoom) {
	camera = Camera(start, end, duration, zoom);
	Renderer.camera = &camera;
}


void Game::UpdateEntities() {
	for (auto ent = Entities.begin(); ent != Entities.end();) {
		if (!(*ent)->destroy) {
			if (!SDL_IsTextInputActive())
				(*ent)->HandleInput();

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
				bool collide = ((*b)->type == BRUSH_SOLID);
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

	ENT_CONSTRUCT_MSG[ENT_PLAYER] = Ents::Player::CONSTRUCT_MSG;

	CVARS["gravity"] = Argument(1800.0);
	CVARS["friction"] = Argument(25.0);
	CVARS["player_speed"] = Argument(300.0);
	CVARS["player_jump"] = Argument(800.0);
	CVARS["timescale"] = Argument(1.0);

	CameraUpdate();
}

void Game::Quit() {
	World.Clear();
}

int Game::CreateEntity(Entity_Type ent_type, std::vector<Argument>& args) {
	std::unique_ptr<Entity> ent = nullptr;
	switch (ent_type) {
	case ENT_PLAYER: ent = std::make_unique<Ents::Player>(); break;

	default: return 0;
	}

	if (!ent->Construct(args)) return 0;
	Entities.push_back(std::move(ent));
	return 1;
}

bool Game::EntityExists(unsigned long ID) {
	for (auto e = Entities.begin(); e != Entities.end(); ++e) {
		if ((*e)->UNIQUE_ID == ID) return true;
	}
	return false;
}

Entity* Game::GetEntityByID(unsigned long ID) {
	for (auto e = Entities.begin(); e != Entities.end(); ++e) {
		if ((*e)->UNIQUE_ID == ID) return e->get();
	}
	return nullptr;
}
