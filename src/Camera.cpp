#include "Camera.hpp"
#include "Game.hpp"

Vec2 Camera::GetPos() {
	Entity* ent;
	double ratio;

	switch (type) {
	case CAMERA_STATIC:
		return static_pos;
		break;
	case CAMERA_FOLLOW:
		ent = Game.GetEntityByID(id_follow);
		if (ent == nullptr) return Vec2(0.0,0.0);
		return ent->pos + (ent->size/2.0);
		break;
	case CAMERA_PATH:
		ratio = timer.GetTime().count()/1000000000.0;
		ratio = duration/4.0 - std::fmod(ratio, duration)/duration;
		ratio = 1.0 - 2.0 * std::abs(ratio);

		return start_pos + (end_pos-start_pos) * ratio;
		break;
	}
}

double Camera::GetZoom() {
	return zoom;
}
