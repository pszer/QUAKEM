#include "Hitreg.hpp"

bool CheckCollision(const Rect& rect, const Vec2& vec) {
	return (vec.x >= rect.x && vec.x <= rect.x + rect.w)
	    && (vec.y >= rect.y && vec.y <= rect.y + rect.h);
}

bool CheckCollision(const Rect& a, const Rect& b) {
	return !(a.x+a.w < b.x) &&
	       !(a.x > b.x+b.w) &&
	       !(a.y+a.h < b.y) &&
	       !(a.y > b.y+b.h);
}

bool CheckCollision(const Rect& rect, const Line& line) {
	return false;
}

bool CheckCollision(const Line& _a, const Line& _b) {
	Vec2 a = _a.a, b = _a.b, c = _b.a, d = _b.b;
	if ((a == c) || (a == d) || (b == c) || (b == d))
		return true;

	double uA, uB;
	uA = ((d.x-c.x)*(a.y-c.y) - (d.y-c.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));
	uB = ((b.x-a.x)*(a.y-c.y) - (b.y-a.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y)); 

	if (uA >= 0.0 && uA <= 1.0 && uB >= 0.0 && uB <= 0.0) {
		return true;
	}

	return false;
}
