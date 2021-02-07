#pragma once

#include "SystemClasses.h"
#include "MapComponents.h"

class MapSystem
{
private:
	//Map component handle
	entt::handle mc_handle;
public:
	MapSystem() {};
	MapSystem(map_data&& map_component, entt::registry& reg, size_t width, size_t height);
	void on_position_update(entt::registry & reg, entt::entity ent);
	const Tile& get_tile(int x, int y);
	entt::entity create_map_entity(int x, int y);
	void set_entity_world_position(int x, int y, const entt::entity& e);
	void add_to_tile(int x, int y, const entt::entity& e);
	void remove_from_tile(int x, int y, const entt::entity& e);
	void render(entt::registry& reg, graphics_context& context, static_sprite_dic& dic);
	void update(entt::registry& reg, SDL_Keycode);
	bool is_entity(int x, int y, const entt::entity& ent);
	bool in_range(int x, int y);
	bool passable(int x, int y);
};

