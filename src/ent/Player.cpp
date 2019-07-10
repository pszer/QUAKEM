#include "ent/Player.hpp"
#include "Game.hpp"

namespace Ents {

const std::string Player::CONSTRUCT_MSG = "x y hp";

void Player::Update() {
	SpawnBullets();

	if (!move_left || !move_right) {
		double speed = GetCVarFloat("player_speed");
		double accel = 18.0;
		if (!on_ground) accel = 3.0;

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

void Player::SpawnBullets() {
	if (shot) {
		if (shot_counter == shot_burst) {
			shot_counter = 0;
			shot = false;
			return;
		}

		if (!shot_counter || burst_timer.GetSeconds() > shot_delay) {
			burst_timer.Reset();
			++shot_counter;
		} else return;

		Vec2 b_vel = Renderer.ReverseTransformVec2(Vec2(Event.mouse_x, Event.mouse_y)) - pos;
		b_vel = b_vel / std::sqrt(b_vel.x*b_vel.x + b_vel.y*b_vel.y); // normalize vector
		b_vel = b_vel * 1000.0; // speed
		b_vel = b_vel + vel; // add on parent player velocity
		Vec2 b_pos = pos + (Hull().Size()/2.0) - Vec2();

		std::vector<Argument> args = {
		  Argument(b_vel.x, "xv"),
		  Argument(b_vel.y, "yv"),
		  Argument(b_pos.x, "x"),
		  Argument(b_pos.y, "y"),
		  Argument(10ll, "dmg"),
		  Argument(0.8, "life"),
		  Argument(ARG_STRING, "player", "team"),
		};

		Game.CreateEntity(ENT_BULLET, args);
	}
}

void Player::Fire() {
	if (cooldown.GetSeconds() > fire_cooldown) {
		cooldown.Reset();
		burst_timer.Reset();
		shot = true;
	}
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
