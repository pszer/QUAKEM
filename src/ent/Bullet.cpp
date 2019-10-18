#include "ent/Bullet.hpp"

#include "Sound.hpp"
#include "Game.hpp"

namespace Ents {

// bullet
Bullet::Bullet(): Entity(ENT_BULLET,
	  Vec2(0.0,0.0), Vec2(16.0,16.0), Vec2(0.0,0.0), TEAM_NULL, true, false)
{
	timer.Start();
}

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
		else if (arg.label=="team") team = StrToTeam(arg.ToString());
	}

	Sound::PlaySound("flyby.wav", 0.3, Sound::_ent_pos_update, UNIQUE_ID);

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




// Rocket
Rocket::Rocket(): Entity(ENT_ROCKET,
	  Vec2(0.0,0.0), Vec2(20.0,20.0), Vec2(0.0,0.0), TEAM_NULL, true, false)
{ }

void Rocket::Update() {
}

void Rocket::Render() {	
	Renderer.RenderFillRect(Hitbox(), {0xFF,0x10,0x10,0xFF});
}

Rect Rocket::Hull() {
	return Hitbox();
}

Rect Rocket::Hitbox() {
	return Rect(pos.x,pos.y,size.x,size.y);
}

const std::string Rocket::CONSTRUCT_MSG = "x y xv yv dmg rad team";
int Rocket::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")    pos.x = arg.ToFloat() - size.x/2.0;
		else if (arg.label=="y")    pos.y = arg.ToFloat() - size.y/2.0;
		else if (arg.label=="xv")   vel.x = arg.ToFloat();
		else if (arg.label=="yv")   vel.y = arg.ToFloat();
		else if (arg.label=="dmg")  damage = arg.ToInt();
		else if (arg.label=="rad")  radius = arg.ToFloat();
		else if (arg.label=="team") team = StrToTeam(arg.ToString());
	}

	return 1;
}

void Rocket::BrushCollision(Brush * brush) {
	if (brush->type == BRUSH_SOLID)	{
		if (!destroy) SpawnExplosion();
		destroy = true;
	}
}

void Rocket::EntityCollision(Entity * entity) {
	if (team != entity->team && entity->team != TEAM_NULL) {
		if (!destroy) SpawnExplosion();
		destroy = true;
	}
}

void Rocket::SpawnExplosion() {
	std::vector<Argument> args = {
		Argument(Hull().Middle().x, "x"),
		Argument(Hull().Middle().y, "y"),
		Argument((long long)damage, "dmg"),
		Argument(radius, "rad"),
		Argument(ARG_STRING, TeamToStr(team), "team")
	};

	Game.CreateEntity(ENT_EXPLOSION, args);
}

std::string Rocket::Info() {
	  return "x:" + std::to_string(pos.x) + "[" + std::to_string(vel.x) + "] y:" + std::to_string(pos.y) +
		  "[" + std::to_string(vel.y) + "]";

}

}
