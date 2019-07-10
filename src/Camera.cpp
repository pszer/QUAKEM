#include "Camera.hpp"
#include "Game.hpp"

Camera::Camera(unsigned long id, double z): type(CAMERA_FOLLOW), id_follow(id), zoom(z)
{
	Entity* ent = Game.GetEntityByID(id_follow);
	if (ent != nullptr) {
		static_pos = ent->pos + (ent->size/2.0);
	}
}

Vec2 Camera::GetPos() {
	Entity* ent;
	double ratio, dt, length;
	double min_dist = Event.win_w < Event.win_h ? Event.win_w : Event.win_h;
	min_dist /= 3.0;
	min_dist *= zoom;
	Vec2 diff;

	switch (type) {
	case CAMERA_STATIC:
		return static_pos;
	case CAMERA_FOLLOW:
		ent = Game.GetEntityByID(id_follow);
		if (ent == nullptr) return Vec2(0.0,0.0);
		end_pos = ent->pos + (ent->size/2.0);
		dt = FrameLimit.deltatime;
		if (dt == 0.0) dt = 1.0;
		diff = end_pos - static_pos;
		if (diff.x*diff.x + diff.y*diff.y >= min_dist*min_dist) {
			length = std::sqrt(diff.x*diff.x + diff.y*diff.y);
			diff = (diff/length) * (length-min_dist);
			static_pos = static_pos + diff;
		} else{
			static_pos = static_pos + diff * dt * 1.5;
		}
		return static_pos;
	case CAMERA_PATH:
		ratio = timer.GetTime().count()/1000000000.0;
		ratio = std::fmod(ratio, duration)/duration;
		ratio = 2.0 * (ratio - 0.5);
		ratio = std::sin( ratio * 3.14159265359 * 0.5  );
		ratio = 1.0 - ratio*ratio;

		return start_pos + ((end_pos-start_pos) * ratio);
	}
}

double Camera::GetZoom() {
	return zoom;
}
