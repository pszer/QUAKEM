#include "Camera.hpp"
#include "Game.hpp"

#include <iostream>
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
		ratio = std::fmod(ratio, duration)/duration;
		ratio = 2.0 * (ratio - 0.5);
		ratio = std::sin( ratio * 3.14159265359 * 0.5  );
		ratio = 1.0 - ratio*ratio;

		return start_pos + ((end_pos-start_pos) * ratio);
		break;
	}
}

double Camera::GetZoom() {
	return zoom;
}
