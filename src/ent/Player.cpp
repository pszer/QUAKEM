#include "ent/Player.hpp"

namespace Ents {

void Player::Update() {
	vel.x /= 1.0 + (5.0 * FrameLimit.deltatime);
	HandleInput();
	vel.y += 400.0 * FrameLimit.deltatime;

	if (hitpoints < 0)
		destroy = true;
}

void Player::Render() {
	Renderer.RenderFillRect(Hitbox(), {0xDF,0xFF,0x00,0xFF});
}

Rect Player::Hitbox() {
	return Rect(pos, size);
}

void Player::HandleInput() {
	auto k = Keys.GetKeyState(PLAYER_LEFT); 
	if (k == KEY_DOWN || k == KEY_HELD)
		MoveLeft();

	k = Keys.GetKeyState(PLAYER_RIGHT);
	if (k == KEY_DOWN || k == KEY_HELD)
		MoveRight();

	k = Keys.GetKeyState(PLAYER_JUMP);
	if (k == KEY_DOWN)
		Jump();
}

void Player::MoveLeft() {
	vel.x = -250;
}

void Player::MoveRight() {
	vel.x = 250;
}

void Player::Jump() {
	if (!on_ground) return;
	vel.y -= 250;
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
