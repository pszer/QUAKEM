#include "ent/Player.hpp"

namespace Ents {

void Player::Update() {
	move_left = false;
	move_right = false;
	HandleInput();

	if (!move_left || !move_right) {
		double speed = GetCVarFloat("player_speed");
		if (move_left && vel.x > -speed) vel.x = -speed;
		else if (move_right && vel.x < speed) vel.x = speed;
	}

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
	move_left = true;
}

void Player::MoveRight() {
	move_right = true;
}

void Player::Jump() {
	if (!on_ground) return;
	double jump = GetCVarFloat("player_jump");
	vel.y -= jump;
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
