#include "ent/Player.hpp"
#include "Game.hpp"

namespace Ents {

const std::string Player::CONSTRUCT_MSG = "x y hp";

void Player::Update() {
	if (!move_left || !move_right) {
		double speed = GetCVarFloat("player_speed");
		if (move_left && vel.x > -speed) vel.x = -speed;
		else if (move_right && vel.x < speed) vel.x = speed;
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
	if (k == KEY_DOWN || k == KEY_HELD)
		Jump();

	k = Keys.GetKeyState(PLAYER_FIRE);
	if (k == KEY_DOWN) Fire();
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
	if (!on_ground) return;
	double jump = GetCVarFloat("player_jump");
	vel.y -= jump;
}


void Player::Fire() {
	Vec2 vel = Renderer.ReverseTransformVec2(Vec2(Event.mouse_x, Event.mouse_y)) - pos;
	vel = vel / std::sqrt(vel.x*vel.x + vel.y*vel.y); // normalize vector
	vel = vel * 1000.0; // speed
	Vec2 bullet_pos = pos + (Hull().Size()/2.0) - Vec2();

	std::vector<Argument> args = {
	  Argument(bullet_pos.x, "x"),
	  Argument(bullet_pos.y, "y"),
	  Argument(vel.x, "xv"),
	  Argument(vel.y, "yv"),
	  Argument(10ll, "dmg"),
	  Argument(0.8, "life"),
	  Argument(ARG_STRING, "player", "team"),
	};

	Game.CreateEntity(ENT_BULLET, args);
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
	return 1;
}

std::string Player::Info() {
	  return "x:" + std::to_string(pos.x) + " y:" + std::to_string(pos.y) +
		  " hp:" + std::to_string(hitpoints) + "/" + std::to_string(max_hitpoints);
};

}
