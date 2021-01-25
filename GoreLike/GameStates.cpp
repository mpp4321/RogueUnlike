#include "GameStates.h"

TestGameState::TestGameState()
{
}

TestGameState::~TestGameState()
{
}

void TestGameState::start(registry& reg)
{
	this->_context.initialize_graphics();
	resource_tree_search_and_add(&this->_sprite_dict, "resources/");
}

void TestGameState::render(const registry& reg)
{
}

void TestGameState::update(registry& reg, float dt)
{
}
