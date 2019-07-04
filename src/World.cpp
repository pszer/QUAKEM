#include "World.hpp"

std::map<std::string, BRUSH_TYPE> STR_TO_BRUSH_TYPE;

void World::Init() {
	STR_TO_BRUSH_TYPE["BRUSH_SOLID"] = BRUSH_SOLID;
	STR_TO_BRUSH_TYPE["BRUSH_NONSOLID"] = BRUSH_NONSOLID;
	STR_TO_BRUSH_TYPE["BRUSH_BACKGROUND"] = BRUSH_BACKGROUND;
	STR_TO_BRUSH_TYPE["BRUSH_FOREGROUND"] = BRUSH_FOREGROUND;
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
