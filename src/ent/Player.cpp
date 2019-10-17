#include "ent/Player.hpp"
#include "Game.hpp"
#include "HUD.hpp"

namespace Ents {

const std::string Player::CONSTRUCT_MSG = "x y hp";

void Player::Update() {
	if (ActiveWeapon() != nullptr)
		ActiveWeapon()->Update();

	if (!move_left || !move_right) {
		double speed = GetCVarFloat("player_speed");
		double accel = 18.0;
		if (!on_ground && !on_ceiling) accel = 3.0;

		if (move_left && vel.x > -speed) vel.x += -speed * FrameLimit.deltatime * accel;
		else if (move_right && vel.x < speed) vel.x += speed * FrameLimit.deltatime * accel;
	}

	if (hitpoints <= 0)
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
	else
		StopMoveLeft();

	k = Keys.GetKeyState(PLAYER_RIGHT);
	if (k == KEY_DOWN || k == KEY_HELD)
		MoveRight();
	else
		StopMoveRight();

	k = Keys.GetKeyState(PLAYER_JUMP);
	if ((k == KEY_DOWN || k == KEY_HELD) && Keys.GetKeyDuration(PLAYER_JUMP) <= 0.2)
		Jump();

	// weapon switch
	for (int i = 0; i <= 9; ++i) {
		std::string str = "+slot";
		str += (char)('0'+i);
		k = Keys.GetKeyState(str);

		if (k == KEY_DOWN)
			SwitchWeapon(i);
	}

	k = Keys.GetKeyState(PLAYER_FIRE);
	if (k == KEY_DOWN) Fire();
	else if (k == KEY_HELD) {
		if (ActiveWeapon() != nullptr && ActiveWeapon()->GetKey("autofire") == 1.0)
				Fire();
	}
}

void Player::MoveLeft() {
	move_left = true;
}

void Player::MoveRight() {
	move_right = true;
}

void Player::StopMoveLeft() {
	move_left = false;
}

void Player::StopMoveRight() {
	move_right = false;
}

void Player::Jump() {
	if ((Game.gravity >= 0.0 && !on_ground) || (Game.gravity < 0.0 && !on_ceiling)) return;
	double jump = GetCVarFloat("player_jump");
	if (Game.gravity >= 0.0)
		vel.y -= jump;
	else    vel.y += jump;
}

void Player::Fire() {
	if (ActiveWeapon() == nullptr) return;

	Vec2 p = Renderer.ReverseTransformVec2(Vec2(Event.mouse_x, Event.mouse_y));
	p = p - pos;
	ActiveWeapon()->Fire(p);
}

int Player::Construct(const std::vector<Argument>& args) {
	for (auto arg : args) {
		if      (arg.label=="x")  pos.x = arg.ToFloat();
		else if (arg.label=="y")  pos.y = arg.ToFloat();
		else if (arg.label=="hp") {
			hitpoints = arg.ToInt();
			max_hitpoints = hitpoints;
		}
	}

	HUD.Add(std::make_unique<HUD_Elements::HP_Bar>(pos, Vec2(60.0,10.0), UNIQUE_ID));

	return 1;
}

std::string Player::Info() {
	  return "x:" + std::to_string(pos.x) + " y:" + std::to_string(pos.y) +
		  " hp:" + std::to_string(hitpoints) + "/" + std::to_string(max_hitpoints);
};

}
