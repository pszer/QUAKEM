#pragma once

#include "Entity.hpp"
#include "Vec2.hpp"

bool CheckCollision(const Rect& rect, const Vec2& vec);
bool CheckCollision(const Rect& a, const Rect& b);
bool CheckCollision(const Rect& rect, const Line& line);
bool CheckCollision(const Line& a, const Line& b);

// returns 1 if there is a collision, 0 if not
// set collide to false to not clip the entities position
int EntityRectCollision(const std::unique_ptr<Entity>& ent, const Rect& rect, bool collide = true);
