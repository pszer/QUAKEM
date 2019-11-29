#include "ent/Medpack.hpp"
#include "Game.hpp"

void Ents::Medpack::Update() {

}

void Ents::Medpack::Render() {
	Rect r = Hull();
	Renderer.RenderTexture("medpack.png", nullptr, &r);
}

void Ents::Medpack::EntityCollision(Entity * entity) {
	if (entity->type == ENT_PLAYER) {
		entity->Heal(heal);
		destroy = true;
	}
}

const std::string Ents::Medpack::CONSTRUCT_MSG = "x y xv yv heal";
int Ents::Medpack::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")  pos.x = arg.ToFloat() - size.x/2.0;
		else if (arg.label=="y")  pos.y = arg.ToFloat() - size.y/2.0;
		else if(arg.label=="xv")  vel.x = arg.ToFloat();
		else if(arg.label=="yv")  vel.y = arg.ToFloat();
		else if (arg.label=="heal") heal = arg.ToInt();
	}
	return 1;

}

std::string Ents::Medpack::Info() {
	return "x:" + std::to_string(pos.x) + " y:" + std::to_string(pos.y) +
	  " heal:" + std::to_string(heal);
}

void Ents::Ammopack::Update() {

}

void Ents::Ammopack::Render() {
	Rect r = Hull();
	Renderer.RenderTexture("ammopack.png", nullptr, &r);

	std::string s = Weapons::SLOT_TO_ICON[slot];
	r.x += 4;
	r.y += 4;
	r.w -= 8;
	r.h -= 8;
	Renderer.RenderTexture(s, nullptr, &r);
}

void Ents::Ammopack::EntityCollision(Entity * entity) {
	if (entity->type == ENT_PLAYER) {
		destroy = true;

		auto wep = entity->GetWeapon(slot);
		if (wep == nullptr) return;
		double m = wep->GetKey("ammo");
		if (m < 0.0) return;
		m += ammo;
		wep->SetKey("ammo", m);
	}
}

const std::string Ents::Ammopack::CONSTRUCT_MSG = "x y xv yv slot ammo";
int Ents::Ammopack::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")  pos.x = arg.ToFloat() - size.x/2.0;
		else if (arg.label=="y")  pos.y = arg.ToFloat() - size.y/2.0;
		else if(arg.label=="xv")  vel.x = arg.ToFloat();
		else if(arg.label=="yv")  vel.y = arg.ToFloat();
		else if (arg.label=="slot") slot = arg.ToInt();
		else if (arg.label=="ammo") ammo = arg.ToInt();
	}
	return 1;

}

std::string Ents::Ammopack::Info() {
	return "x:" + std::to_string(pos.x) + " y:" + std::to_string(pos.y) +
	  " ammo:" + std::to_string(ammo);
}
