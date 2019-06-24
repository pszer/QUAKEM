#include "Vec2.hpp"

Vec2 Rect::Pos() {
	return Vec2(x,y);
}

Vec2 Rect::Size() {
	return Vec2(w,h);
}

void Rect::SetPos(const Vec2& v) {
	x = v.x;
	y = v.y;
}

void Rect::SetSize(const Vec2& s) {
	w = s.x;
	h = s.y;
}

SDL_Rect Rect::ToSDLRect() {
	return {(int)x,(int)y,(int)w,(int)h};
}

Vec2 operator+(const Vec2& a, const Vec2& b) {
	return Vec2(a.x + b.x, a.y + b.y);
}
Vec2 operator-(const Vec2& a, const Vec2& b) {
	return Vec2(a.x - b.x, a.y - b.y);
}
Vec2 operator*(const Vec2& a, const double& s) {
	return Vec2(a.x * s, a.y * s);
}
Vec2 operator/(const Vec2& a, const double& s) {
	return Vec2(a.x / s, a.y / s);
}
