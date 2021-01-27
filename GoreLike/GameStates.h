#pragma once

#include "SDL.h"
#include "entt.hpp"
#include "SystemClasses.h"
#include "FileUtil.h"
#include "Map.h"

using namespace entt;

class GameState {
public:
	virtual void start(registry & reg) = 0;
	virtual void render(registry & reg) = 0;
	virtual void update(registry & reg, float dt) = 0;
};

class TestGameState : public GameState {
private:
	GraphicsContext _context{};
	StaticSpriteDictionary _sprite_dict{};
	SDL_Keycode polled_key;
	bool quit = false;
public:
	TestGameState();
	~TestGameState();
	StaticSpriteDictionary& get_sprite_dict();
	void start(registry& reg);
	void render(registry & reg);
	void update(registry & reg, float dt);
};


