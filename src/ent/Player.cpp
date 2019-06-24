#include "ent/Player.hpp"

namespace Ents {

void Player::Update() {
	if (hitpoints < 0)
		destroy = true;
}

void Player::Render() {
	Renderer.RenderFillRect(Hitbox(), {0xDF,0xFF,0x00,0xFF});
}

Rect Player::Hitbox() {
	return Rect(pos, size);
}

Rect Player::Hull() {
	return Hitbox();
}

int Player::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")  pos.x = arg.ToFloat();
		else if (arg.label=="y")  pos.y = arg.ToFloat();
		else if (arg.label=="w")  size.x = arg.ToFloat();
		else if (arg.label=="h")  size.y = arg.ToFloat();
		else if (arg.label=="hp") hitpoints = arg.ToInt();
	}
	return 1;
}

};
