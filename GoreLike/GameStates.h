#pragma once

#include "SDL.h"
#include "entt.hpp"
#include "SystemClasses.h"
#include "tiled_map_util.h"
#include "GUISystem.h"
#include "FileUtil.h"
#include "MapComponents.h"
#include "entity_json_utils.h"
#include "MapSystem.h"
#include "gui_help.hpp"

using namespace entt;

class GameState {
public:
	virtual void start(registry & reg) = 0;
	virtual void render(registry & reg) = 0;
	virtual void update(registry & reg, SDL_Event* ev) = 0;
};

class TestGameState : public GameState {
private:
	//Singleton systems to utilize context variables
	const int FPS = 144;
	MapSystem map_system;
	GUISystem gui_system;

	graphics_context _context{};
	static_sprite_dic _sprite_dict{};
	SDL_Keycode polled_key;
	std::pair<int, int> mouse_loc = { 0,0 };

	Uint64 time_now = SDL_GetPerformanceCounter();
	Uint64 time_last = 0;

	bool quit = false;
public:
	TestGameState(entt::registry& reg);
	~TestGameState();
	static_sprite_dic& get_sprite_dict();
	void start(registry& reg);
	void tick(registry& reg, float dt);
	void render(registry & reg);
	void update(registry & reg, SDL_Event*);
};


