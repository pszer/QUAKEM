#include "ent/Bullet.hpp"

namespace Ents {

void Bullet::Update() {
	if (timer.GetSeconds() > lifetime) destroy = true;
}

void Bullet::Render() {	
	Renderer.RenderFillRect(Hitbox(), {0xFF,0xF6,0x7A,0xFF});
}

Rect Bullet::Hull() {
	return Rect(pos.x + size.x/2.0, pos.y + size.y/2.0, 1.0, 1.0);
}

Rect Bullet::Hitbox() {
	return Rect(pos.x,pos.y,size.x,size.y);
}

const std::string Bullet::CONSTRUCT_MSG = "x y xv yv dmg life team";
int Bullet::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")    pos.x = arg.ToFloat() - size.x/2.0;
		else if (arg.label=="y")    pos.y = arg.ToFloat() - size.y/2.0;
		else if (arg.label=="xv")   vel.x = arg.ToFloat();
		else if (arg.label=="yv")   vel.y = arg.ToFloat();
		else if (arg.label=="dmg")  damage = arg.ToInt();
		else if (arg.label=="life") lifetime = arg.ToFloat();
		else if (arg.label=="team") {
			if (arg.ToString() == "player") team = TEAM_PLAYER;
			else if (arg.ToString() == "enemy") team = TEAM_ENEMY;
		}
	}

	return 1;
}

void Bullet::BrushCollision(Brush * brush) {
	if (brush->type == BRUSH_SOLID) destroy = true;
}

void Bullet::EntityCollision(Entity * entity) {
	if ((team == TEAM_PLAYER && entity->team == TEAM_ENEMY) ||
	    (team == TEAM_ENEMY && entity->team == TEAM_PLAYER))
	{
		entity->hitpoints -= damage;
		destroy = true;
	}
}

std::string Bullet::Info() {
	  return "x:" + std::to_string(pos.x) + "[" + std::to_string(vel.x) + "] y:" + std::to_string(pos.y) +
		  "[" + std::to_string(vel.y) + "]";
}

}
