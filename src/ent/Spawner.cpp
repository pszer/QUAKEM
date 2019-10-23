#include "ent/Spawner.hpp"

#include "Game.hpp"

namespace Ents {

Spawner::Spawner(): Entity(ENT_SPAWNER,
	  Vec2(0.0,0.0), Vec2(0.0,0.0), Vec2(0.0,0.0), TEAM_NULL, false, false)
{
	timer.Start();
}

void Spawner::Update() {
	if (quantity != -1 && counter >= quantity) {
		destroy = true;
	} else if (timer.GetSeconds() >= rate) {
		timer.Reset();
		Spawn();
	}
}

void Spawner::Render() {	
}

Rect Spawner::Hull() {
	return Hitbox();
}

Rect Spawner::Hitbox() {
	return Rect(pos.x,pos.y,size.x,size.y);
}

const std::string Spawner::CONSTRUCT_MSG = "ent_ rate_ quantity_ (...entity args...)";
int Spawner::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="ent_") pos.x = StrToEnt(arg.ToString());
		else if (arg.label=="rate_") rate = arg.ToFloat();
		else if (arg.label=="quantity_") quantity = arg.ToInt();
		else {
			ent_args.push_back(arg);	
		}
	}

	return 1;
}

void Spawner::Spawn() {
	++counter;
	//destroy if spawner is useless
	if (!Game.CreateEntity(type, ent_args)) destroy = true;
}

std::string Spawner::Info() {
	return "ent:["+EntToStr(type) + "] rate:["+std::to_string(rate)+
	  "] + quantity:["+std::to_string(quantity)+"] counter:["+std::to_string(counter)+"]";
}

}
