#include "Game.hpp"
#include "EntList.hpp"

struct Game Game;

void Game::Update() {
	UpdateEntities();
	UpdatePhysics();
	World.CollideWithEntities();
	World.Update();
	CameraWheelScroll();
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

void Game::CameraWheelScroll() {
	scroll_count += Event.wheel_y * 0.035;

	double d = scroll_count * std::min(FrameLimit.deltatime * 8.0, 0.2);
	scroll_change += d;
	scroll_count -= d;
	if (scroll_count < 0.02 && scroll_count > -0.02) scroll_count = 0.0;
	if (scroll_change < 0.0) scroll_change = 0.0;

	if (Event.wheel_y) {
		scroll_timer.Stop();
	} else {
		scroll_timer.Start();
	}

	if (scroll_timer.GetSeconds() > 0.4) {
		scroll_change -= (scroll_change - 1.0)*std::min(FrameLimit.deltatime*2.0, 0.2);
		if (scroll_change < 1.005 && scroll_change > 0.995) {
			scroll_count = 0.0;
			scroll_change = 1.0;
		}
	}

	camera.zoom = camera_zoom * scroll_change;
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
	for (auto ent = Entities_Queue.begin(); ent != Entities_Queue.end(); ++ent) {
		Entities.push_back(std::move(*ent));
	}
	Entities_Queue.clear();

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
		if ((*ent)->collide) {
			for (auto b = World.Brushes.begin(); b != World.Brushes.end(); ++b) {
				if (CheckCollision((*b)->rect, (*ent)->Hull())) {
					(*ent)->BrushCollision(b->get());
					if ((*ent)->destroy) goto nextent;

					if ((*b)->check_collision)
						(*b)->CollideFunc(ent->get());
				}
			}

			for (auto ent2 = Entities.begin(); ent2 != Entities.end(); ++ent2) {
				if (ent == ent2) continue;
				if (CheckCollision((*ent2)->Hitbox(), (*ent)->Hitbox())) {
					(*ent)->EntityCollision(ent2->get());
					if ((*ent)->destroy) goto nextent;
				}
			}
		}

		if ((*ent)->physics) {
			ApplyGravity(ent->get());
			double max_speed = GetCVarFloat("max_speed");
			Vec2& vel = (*ent)->vel;
			if (vel.x * vel.x + vel.y * vel.y > max_speed * max_speed) {
				double mag = std::sqrt(vel.x * vel.x + vel.y * vel.y);
				vel = vel / mag;
				vel = vel * max_speed;
			}

			if ((*ent)->on_ground || (*ent)->on_ceiling)
				(*ent)->vel.x /= 1.0 + (friction * FrameLimit.deltatime);

			(*ent)->ResetFlags();
			for (auto b = World.Brushes.begin(); b != World.Brushes.end(); ++b) {
				if ((*b)->type != BRUSH_SOLID) continue;
				EntityRectCollision(ent->get(), (*b)->rect);
			}
			(*ent)->UpdatePos();
		} else {
			(*ent)->UpdatePos();
		}

		nextent: NULL;
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
	STR_TO_ENT_TYPE["ENT_BULLET"] = ENT_BULLET;
	STR_TO_ENT_TYPE["ENT_ENEMY_WALKER"] = ENT_ENEMY_WALKER;

	ENT_CONSTRUCT_MSG[ENT_PLAYER] = Ents::Player::CONSTRUCT_MSG;
	ENT_CONSTRUCT_MSG[ENT_BULLET] = Ents::Bullet::CONSTRUCT_MSG;
	ENT_CONSTRUCT_MSG[ENT_ENEMY_WALKER] = Ents::Walker::CONSTRUCT_MSG;

	CVARS["followspeed"] = Argument(3.5);
	CVARS["gravity"] = Argument(1800.0);
	CVARS["max_speed"] = Argument(3500.0);
	CVARS["friction"] = Argument(25.0);
	CVARS["player_speed"] = Argument(420.0);
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
	case ENT_BULLET: ent = std::make_unique<Ents::Bullet>(); break;
	case ENT_ENEMY_WALKER: ent = std::make_unique<Ents::Walker>(); break;

	default: return 0;
	}

	if (!ent->Construct(args)) return 0;
	Entities_Queue.push_back(std::move(ent));
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
