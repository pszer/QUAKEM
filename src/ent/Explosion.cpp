#include "ent/Explosion.hpp"

#include "Sound.hpp"

namespace Ents {

Explosion::Explosion(): Entity(ENT_EXPLOSION,
	  Vec2(0.0,0.0), Vec2(20.0,20.0), Vec2(0.0,0.0), TEAM_NULL, true, false)
{
	Sound::PlaySound("explode.wav", 0.5, Sound::_ent_pos_update, UNIQUE_ID);
}

void Explosion::Update() {
	if (lived) destroy = true;
	lived = true;
}

void Explosion::Render() {	
	Renderer.RenderFillRect(Hitbox(), {0xFF,0xFF,0xFF,0xFF});
}

Rect Explosion::Hull() {
	return Hitbox();
}

Rect Explosion::Hitbox() {
	return Rect(pos.x,pos.y,size.x,size.y);
}

const std::string Explosion::CONSTRUCT_MSG = "x y dmg rad team";
int Explosion::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")    pos.x = arg.ToFloat();
		else if (arg.label=="y")    pos.y = arg.ToFloat();
		else if (arg.label=="dmg")  damage = arg.ToInt();
		else if (arg.label=="rad")  radius = arg.ToFloat();
		else if (arg.label=="team") team = StrToTeam(arg.ToString());
	}

	pos.x -= radius;
	pos.y -= radius;
	size.x = radius*2;
	size.y = radius*2;

	return 1;
}

void Explosion::EntityCollision(Entity * entity) {
	if (team != entity->team && entity->team != TEAM_NULL) {
		entity->Heal(-damage);
	}
	if (entity->team != TEAM_NULL) {
		Vec2 diff = entity->Hull().Middle() - Hull().Middle();
		std::cout << diff.x << " " << diff.y << std::endl;
		double length = std::sqrt(diff.x*diff.x + diff.y*diff.y);;
		entity->Push((diff / (length + 1.0)) * 9.0 * damage);
	}
}

std::string Explosion::Info() {
	  return "x:" + std::to_string(pos.x) + "[" + std::to_string(vel.x) + "] y:" + std::to_string(pos.y) +
		  "[" + std::to_string(vel.y) + "]";

}

}
