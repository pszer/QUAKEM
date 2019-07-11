#include "br/BRUSH_PUSH.hpp"

void Brushes::Push::CollideFunc(Entity * ent) {
	Vec2& v = ent->vel;

	double xf = GetValue("xforce"), yf = GetValue("yforce");
	double xf2 = xf * xf, yf2 = yf * yf;
	if (xf < 0.0) xf2 = -xf2;
	if (yf < 0.0) yf2 = -yf2;

	if ( ((xf > 0.0)&&(v.x <= xf)) || ((xf < 0.0)&&(v.x >= xf)) ) {
		v.x += xf * FrameLimit.deltatime;
	}
	if ( ((yf > 0.0)&&(v.y <= yf)) || ((yf < 0.0)&&(v.y >= yf)) ) {
		v.y += yf * FrameLimit.deltatime;
	}
}
