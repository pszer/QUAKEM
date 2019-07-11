#include "Hitreg.hpp"
#include "Timer.hpp"

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
	const Vec2 pos = rect.Pos(), size = rect.Size();

	const Vec2 topleft = pos, topright = pos + Vec2(size.x, 0.0),
	        bottomleft = topleft + Vec2(0.0, size.y), bottomright = bottomleft + Vec2(size.x, 0.0);

	// lines of the rectangle
	const Line top = Line(topleft, topright),
	           bottom = Line(bottomleft, bottomright),
	           left = Line(topleft, bottomleft),
	           right = Line(topright, bottomright);

	// check one by one so not every side has to be calculated if there is a collision
	if (CheckCollision(line, top)) return true;
	if (CheckCollision(line, bottom)) return true;
	if (CheckCollision(line, left)) return true;
	if (CheckCollision(line, right)) return true;

	return false;
}

bool CheckCollision(const Line& _a, const Line& _b) {
	const Vec2& a = _a.a, &b = _a.b, &c = _b.a, &d = _b.b;

	Rect P = Rect(a.x, a.y, b.x-a.x, b.y-a.y).Absolute(),
	     Q = Rect(c.x, c.y, d.x-c.x, d.y-c.y).Absolute();
	if (!CheckCollision(P,Q)) return false;

	// check overlapping points
	if ((a == c) || (a == d) || (b == c) || (b == d))
		return true;

	// check parallel
	if ( ((a.x - b.x) * (c.y - d.y) + (a.y - b.y) * (d.x - c.x) == 0.0) && 
	     ((a.x - d.x) * (c.y - b.y) + (a.y - d.y) * (b.x - c.x) == 0.0) ) return true;

	double uA, uB;
	uA = ((d.x-c.x)*(a.y-c.y) - (d.y-c.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y));
	uB = ((b.x-a.x)*(a.y-c.y) - (b.y-a.y)*(a.x-c.x)) / ((d.y-c.y)*(b.x-a.x) - (d.x-c.x)*(b.y-a.y)); 

	if (uA >= 0.0 && uA <= 1.0 && uB >= 0.0 && uB <= 1.0) {
		return true;
	}

	return false;
}

int EntityRectCollision(Entity* ent, const Rect rect, bool collide) {
	const Rect hull = ent->Hull();	
	Rect guaranteed_rect = Rect();
	Vec2 dvel = ent->vel * FrameLimit.deltatime;

	// guaranteed_rect is the box that bounds the entity
	// as well as the entity after its transformed by its velocity
	// if the rect isnt in this guaranteed_rect, there will not
	// be a collision

	if (ent->vel.x >= 0.0) {
		guaranteed_rect.x = hull.x;
		guaranteed_rect.w = hull.w + dvel.x;
	} else {
		guaranteed_rect.x = hull.x + dvel.x;
		guaranteed_rect.w = hull.w - dvel.x;
	}
	if (ent->vel.y >= 0.0) {
		guaranteed_rect.y = hull.y;
		guaranteed_rect.h = hull.h + dvel.y;
	} else {
		guaranteed_rect.y = hull.y + dvel.y;
		guaranteed_rect.h = hull.h - dvel.y;
	}

	if (!CheckCollision(hull, guaranteed_rect)) return 0;

	const Vec2 r_pos = rect.Pos(), r_size = rect.Size();
	const Vec2 r_topleft = r_pos,
	           r_topright = r_pos + Vec2(r_size.x, 0.0),
	           r_bottomleft = r_topleft + Vec2(0.0, r_size.y),
	           r_bottomright = r_bottomleft + Vec2(r_size.x, 0.0);
	const Vec2 e_topleft = hull.Pos(),
	           e_topright = e_topleft + Vec2(hull.w, 0.0),
	           e_bottomleft = e_topleft + Vec2(0.0, hull.h),
	           e_bottomright = e_bottomleft + Vec2(hull.w, 0.0);

	// collision with top of the brush.
	if (CheckCollision(
	  // bottom-left velocity line of entity, extended top of brush
	  Line(e_bottomleft, e_bottomleft + dvel), Line(r_topleft - Vec2(hull.w, 0.0), r_topright)) ||
	    CheckCollision(
	  // bottom-right velocity line of entity, extended top of brush
	  Line(e_bottomright, e_bottomright + dvel), Line(r_topleft, r_topright + Vec2(hull.w, 0.0) ))
	) {
		double collision_depth = hull.y + hull.h + dvel.y - r_pos.y;
		if (collision_depth > 0.0) {
			ent->pos.y = r_pos.y - hull.h;
			ent->vel.y = 0.0;
			ent->on_ground = true;
		}
	}
	// collision with bottom of the brush.
	else if (CheckCollision(
	  // bottom-left velocity line of entity, extended top of brush
	  Line(e_topleft, e_topleft + dvel), Line(r_bottomleft - Vec2(hull.w, 0.0), r_bottomright)) ||
	    CheckCollision(
	  // bottom-right velocity line of entity, extended top of brush
	  Line(e_topright, e_topright + dvel), Line(r_bottomleft, r_bottomright + Vec2(hull.w, 0.0) ))
	) {
		double collision_depth = hull.y + dvel.y - r_pos.y - r_size.y;
		if (collision_depth < 0.0) {
			ent->pos.y = r_pos.y + r_size.y;
			ent->vel.y = 0.0;
			ent->on_ceiling = true;
		}
	}

	// collision with left of the brush.
	if (CheckCollision(
	  // top-right velocity line of entity, extended side of brush
	  Line(e_topright, e_topright + dvel), Line(r_topleft - Vec2(0.0, hull.h), r_bottomleft)) ||
	    CheckCollision(
	  // bottom-right velocity line of entity, extended side of brush
	  Line(e_bottomright, e_bottomright + dvel), Line(r_topleft, r_bottomleft + Vec2(0.0, hull.h) ))
	) {
		double collision_depth = hull.x + hull.w + dvel.x - r_pos.x;
		if (collision_depth > 0.0) {
			ent->pos.x = r_pos.x - hull.w;
			ent->vel.x = 0.0;
			ent->on_leftwall = true;
		}
	}
	// collision with right of the brush.
	else if (CheckCollision(
	  // top-left velocity line of entity, extended side of brush
	  Line(e_topleft, e_topleft + dvel), Line(r_topright - Vec2(0.0, hull.h), r_bottomright)) ||
	    CheckCollision(
	  // bottom-left velocity line of entity, extended side of brush
	  Line(e_bottomleft, e_bottomleft + dvel), Line(r_topright, r_bottomright + Vec2(0.0, hull.h) ))
	) {
		double collision_depth = hull.x + dvel.x - (r_pos.x + r_size.x);
		if (collision_depth < 0.0) {
			ent->pos.x = r_pos.x + r_size.x;
			ent->vel.x = 0.0;
			ent->on_rightwall = true;
		}
	}

	return 1;
}
