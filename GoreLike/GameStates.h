#pragma once

#include "SDL.h"
#include "entt.hpp"
#include "SystemClasses.h"
#include "tiled_map_util.h"
#include "FileUtil.h"
#include "MapComponents.h"
#include "MapSystem.h"

using namespace entt;

class GameState {
public:
	virtual void start(registry & reg) = 0;
	virtual void render(registry & reg) = 0;
	virtual void update(registry & reg, float dt) = 0;
};

class TestGameState : public GameState {
private:
	//Singleton systems to utilize context variables
	MapSystem map_system;

	graphics_context _context{};
	static_sprite_dic _sprite_dict{};
	SDL_Keycode polled_key;
	bool quit = false;
public:
	TestGameState(entt::registry& reg);
	~TestGameState();
	static_sprite_dic& get_sprite_dict();
	void start(registry& reg);
	void render(registry & reg);
	void update(registry & reg, float dt);
};


