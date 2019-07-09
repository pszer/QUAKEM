#pragma once

#include "Vec2.hpp"
#include "Timer.hpp"

enum Camera_Type { CAMERA_STATIC , CAMERA_FOLLOW , CAMERA_PATH };

struct Camera {
	Camera(): type(CAMERA_STATIC), static_pos(0.0,0.0), zoom(1.0) { }

	// static camera constructor
	Camera(Vec2 _pos, double z=1.0): type(CAMERA_STATIC), static_pos(_pos), zoom(z) { }
	// entity following camera constructor
	Camera(unsigned long id, double z=1.0): type(CAMERA_FOLLOW), id_follow(id), zoom(z) { }
	// path following camera constructor
	Camera(Vec2 start, Vec2 end, double dur, double z=1.0): type(CAMERA_PATH),
	  start_pos(start), end_pos(end), duration(dur), zoom(z) { timer.Start(); }

	Camera_Type type;

	double zoom;
	Vec2 static_pos;
	unsigned long id_follow; // follows entity by ID
	Vec2 start_pos, end_pos;
	double duration;

	Timer timer;
	Vec2 GetPos();
	double GetZoom();
};
