#pragma once

#include <memory>
#include <map>
#include <vector>

#include "Vec2.hpp"
#include "Render.hpp"

struct Entity;

// BRUSH_SOLID's block all entitites
// BRUSH_NONSOLID's don't block entities
// BRUSH_BACKGROUND's don't block entities and they are drawn in the background
// BRUSH_FOREGROUND's don't block entities and they are drawn in the foreground
//
// entity collisions are checked for if check_collision == true
//
// update() is called every frame
//
// std::map keys is used to specify properties for special brushes like push brushes
enum BRUSH_TYPE { BRUSH_SOLID , BRUSH_NONSOLID , BRUSH_BACKGROUND , BRUSH_FOREGROUND ,
                  BRUSH_PUSH , BRUSH_HURT };
extern std::map<std::string, BRUSH_TYPE> STR_TO_BRUSH_TYPE;

struct Brush {
	// Rect BRUSH_TYPE texture scale offset
	Brush(Rect _rect, BRUSH_TYPE _type, const std::string& tex="",
	  Vec2 scale = Vec2(1.0,1.0), Vec2 offset = Vec2(0.0,0.0),
	  std::map<std::string,double> _k=std::map<std::string,double>())
	: rect(_rect), type(_type), texture(tex),
	  texture_scale(scale), texture_offset(offset), keys(_k) { }

	Rect rect;
	BRUSH_TYPE type;

	std::string texture = "";
	Vec2 texture_scale, texture_offset;
	std::map<std::string, double> keys;
	double GetValue(const std::string& str);

	bool check_collision = false;

	virtual void Render();
	virtual void CollideFunc(Entity * ent) { }
	virtual void Update() { }
};

struct World {
	static void Init();

	void Update();
	void CollideWithEntities();
	void Clear();

	void RenderBackground();
	void RenderMiddle();
	void RenderForeground();

	void CreateBrush(Rect _rect, BRUSH_TYPE _type, const std::string& tex,
	  Vec2 scale, Vec2 offset, std::map<std::string, double> keys);

	std::vector<std::unique_ptr<Brush>> Brushes;
};

#include "Entity.hpp"
