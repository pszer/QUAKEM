#include "World.hpp"

#include "br/BRUSH_PUSH.hpp"

std::map<std::string, BRUSH_TYPE> STR_TO_BRUSH_TYPE;

void World::Init() {
	STR_TO_BRUSH_TYPE["BRUSH_SOLID"] = BRUSH_SOLID;
	STR_TO_BRUSH_TYPE["BRUSH_NONSOLID"] = BRUSH_NONSOLID;
	STR_TO_BRUSH_TYPE["BRUSH_BACKGROUND"] = BRUSH_BACKGROUND;
	STR_TO_BRUSH_TYPE["BRUSH_FOREGROUND"] = BRUSH_FOREGROUND;
	STR_TO_BRUSH_TYPE["BRUSH_PUSH"] = BRUSH_PUSH;
}

void World::CreateBrush(Rect rect, BRUSH_TYPE type, const std::string& tex, Vec2 scale, Vec2 offset,
  std::map<std::string, double> keys)
{
	if (type <= BRUSH_FOREGROUND) {
		Brushes.push_back(std::make_unique<Brush>(rect, type, tex, scale, offset, std::move(keys)));
		return;
	}

	std::unique_ptr<Brush> br;
	switch (type) {
	case BRUSH_PUSH:
		br = std::make_unique<Brushes::Push>(rect, tex, scale, offset, std::move(keys)); break;
	default: return;
	}

	if (br != nullptr) Brushes.push_back(std::move(br));
}

double Brush::GetValue(const std::string& str) {
	auto f = keys.find(str);
	if (f != keys.end()) return f->second;
	return 0.0;
}

void Brush::Render() {
	if (texture.empty()) return;
	Renderer.RenderTiledTexture(texture, rect, texture_scale, texture_offset);
}

void World::Update() {
	for (auto b = Brushes.begin(); b != Brushes.end(); ++b) {
		(*b)->Update();
	}
}

void World::CollideWithEntities() {
	for (auto b = Brushes.begin(); b != Brushes.end(); ++b) {
		if (!(*b)->check_collision) continue;
		// collision check
	}
}

void World::Clear() {
	Brushes.clear();
}

void World::RenderBackground() {
	for (auto b = Brushes.begin(); b != Brushes.end(); ++b) {
		if ((*b)->type == BRUSH_BACKGROUND) (*b)->Render();
	}
}

void World::RenderMiddle() {
	for (auto b = Brushes.begin(); b != Brushes.end(); ++b) {
		if ((*b)->type != BRUSH_BACKGROUND &&
		    (*b)->type != BRUSH_FOREGROUND)
			(*b)->Render();
	}
}

void World::RenderForeground() {
	for (auto b = Brushes.begin(); b != Brushes.end(); ++b) {
		if ((*b)->type == BRUSH_FOREGROUND) (*b)->Render();
	}
}
