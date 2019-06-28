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
	double denominator = ((b.x - a.y) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
	double numerator1 = ((a.y - c.y) * (d.y - c.x)) - ((a.x - c.x) * (d.y - c.y));
	double numerator2 = ((a.y - c.y) * (b.y - a.x)) - ((a.x - c.x) * (b.y - a.y));

	if (denominator == 0.0) return (numerator1 == 0.0) && (numerator2 == 0.0);

	double r = numerator1 / denominator;
	double s = numerator2 / denominator;

	return (r >= 0.0 && r <= 1.0) && (s >= 0.0 && s <= 1.0);
}
