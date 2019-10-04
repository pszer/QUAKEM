#pragma once

#include <vector>
#include <memory>

#include "Hitreg.hpp"
#include "Commands.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Sound.hpp"

extern struct Game {
	void Init();
	void Quit();

	std::vector< std::unique_ptr<Entity> > Entities;
	void UpdateEntities();
	void RenderEntities();

	void UpdatePhysics();
	void ApplyGravity(Entity* ent);

	double gravity=0.0, friction=5.0;

	std::map<std::string, Entity_Type> STR_TO_ENT_TYPE;
	std::map<Entity_Type, std::string> ENT_CONSTRUCT_MSG;

	// 1 for success, 0 for error
	int CreateEntity(Entity_Type ent, std::vector<Argument>& args);

	// returns true if entity with ID exists
	bool EntityExists(unsigned long ID);

	// returns nullptr if entity doesnt exist
	// otherwise returns entity pointer to it
	Entity* GetEntityByID(unsigned long ID);

	bool world_loaded = false;
	struct World World;

	enum GAME_CAMERA_MODE { GAME_CAMERA_DEFAULT , GAME_CAMERA_STATIC , GAME_CAMERA_FOLLOW_PLAYER ,
	  GAME_CAMERA_FOLLOW_ENT , GAME_CAMERA_PATH };
	GAME_CAMERA_MODE camera_mode = GAME_CAMERA_DEFAULT;
	
	// fill these data to update camera during CameraUpdate
	unsigned long camera_id = 0;
	Vec2 camera_pos = Vec2(0.0,0.0);
	Vec2 camera_start = Vec2(0.0,0.0), camera_end = Vec2(0.0,0.0);
	double camera_duration = 1.0;
	double camera_zoom = 1.0;

	Camera camera;
	void CameraUpdate();
	void CameraDefault();
	void CameraFollowEntity(unsigned long ID, double zoom = 1.0);
	void CameraFollowPlayer(double zoom = 1.0);
	void CameraStatic(Vec2 pos, double zoom = 1.0);
	void CameraPath(Vec2 start, Vec2 end, double duration, double zoom = 1.0);

	Timer scroll_timer;
	double scroll_count = 0.0, scroll_change = 1.0;
	void CameraWheelScroll();

	void Update();
	void Render();
private:
	std::vector< std::unique_ptr<Entity> > Entities_Queue;
} Game;
