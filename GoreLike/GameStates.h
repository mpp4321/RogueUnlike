#pragma once

#include "SDL.h"
#include "entt.hpp"
#include "SystemClasses.h"
#include "FileUtil.h"

using namespace entt;

class GameState {
public:
	virtual void start(registry & reg) = 0;
	virtual void render(const registry & reg) = 0;
	virtual void update(registry & reg, float dt) = 0;
};

class TestGameState : public GameState {
private:
	GraphicsContext _context{};
	StaticSpriteDictionary _sprite_dict{};
public:
	TestGameState();
	~TestGameState();
	void start(registry& reg);
	void render(const registry & reg);
	void update(registry & reg, float dt);
};


