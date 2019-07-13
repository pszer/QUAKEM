#include "ent/Enemy.hpp"
#include "Game.hpp"

namespace Ents {

const std::string EnemyBase::CONSTRUCT_MSG = "";

bool EnemyBase::CanSeePlayer() {
	for (auto ent = Game.Entities.begin(); ent != Game.Entities.end(); ++ent) {
		if ((*ent)->type != ENT_PLAYER) continue;
		else {
			bool seen = CanSeeEntity(ent->get());
			if (seen) player_id = (*ent)->UNIQUE_ID;
			return seen;
		}
	}
	return false;
}


bool EnemyBase::CanSeeEntity(int UNIQUE_ID) {
	for (auto ent = Game.Entities.begin(); ent != Game.Entities.end(); ++ent) {
		if ((*ent)->UNIQUE_ID == UNIQUE_ID) return CanSeeEntity(ent->get());
	}
	return false;
}

bool EnemyBase::CanSeeEntity(Entity * ent) {
	Line sight = Line( Hull().Middle() , ent->Hull().Middle() );
	for (auto b = Game.World.Brushes.begin(); b != Game.World.Brushes.end(); ++b) {
		if ((*b)->type != BRUSH_SOLID)
			continue;
		else if (CheckCollision((*b)->rect, sight))
			return false;
	}
	return true;
}

void Walker::Update() {
	if (CanSeePlayer()) {
		for (auto ent = Game.Entities.end()-1;; --ent) {
			if ((*ent)->UNIQUE_ID < player_id) break;
			else if ((*ent)->UNIQUE_ID == player_id) {
				if ((*ent)->pos.x < pos.x)
					vel.x = -200.0;
				else
					vel.x = 200.0;
			}

			if (ent == Game.Entities.begin()) break;
		}
	}

	if (hitpoints < 0) {
		destroy = true;

		std::vector<Argument> args = {
		  Argument(Hull().Middle().x, "x"),
		  Argument(Hull().Middle().y, "y"),
		  Argument(0.0, "xv"),
		  Argument(-600.0, "yv"),
		  Argument(50ll, "heal"),
		};

		Game.CreateEntity(ENT_MEDPACK, args);
	}
}

void Walker::Render() {
	Renderer.RenderFillRect(Hitbox(), {0xFF,0x00,0x00,0xFF});
}

void Walker::EntityCollision(Entity * entity) {
	if (entity->team == TEAM_PLAYER) {
		if (rate_timer.GetSeconds() < rate) return;

		rate_timer.Reset();

		entity->hitpoints -= damage;

		if (entity->pos.x < Hull().Middle().x)
			entity->vel = entity->vel + Vec2(-200.0, -300.0);
		else
			entity->vel = entity->vel + Vec2(200.0, -300.0);
	}
}
	
const std::string Walker::CONSTRUCT_MSG = "x y hp dmg rate";
int Walker::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")  pos.x = arg.ToFloat();
		else if (arg.label=="y")  pos.y = arg.ToFloat();
		else if (arg.label=="hp") {
			hitpoints = arg.ToInt();
			max_hitpoints = hitpoints;
		}else if(arg.label=="dmg")
			damage = arg.ToInt();
		else if (arg.label=="rate")
			rate = arg.ToFloat();
	}
	return 1;
}

}
