#pragma once

#include "SystemClasses.h"
#include "MapComponents.h"
#include "tmxlite/Map.hpp"
#include <filesystem>
#include "fmt/format.h"
#include "tiled_map_util.h"

class MapSystem
{
private:
	//Map component handle
	entt::handle mc_handle;
	entt::entity _player_id;
public:
	MapSystem() : _player_id(entt::null) {};
	MapSystem(map_data&& map_component, entt::registry& reg, entt::entity player,
		size_t width, size_t height,
		size_t player_x, size_t player_y);
	static void on_destroy(entt::registry& reg, entt::entity ent);
	static void on_position_update(entt::registry & reg, entt::entity ent);
	const Tile& get_tile(int x, int y);
	static entt::entity create_map_entity(entt::registry& reg, int x, int y);
	/* create_map_entity based on some abstract "id" will load based on json soon*/
	static entt::entity create_map_entity(entt::registry& reg, int x, int y, std::string id);
	void load_by_map_data(map_data&& map_component, size_t player_x, size_t player_y);
	void load_static_map_by_id(const std::string& id, size_t player_start_x, size_t player_start_y);
	void set_entity_world_position(int x, int y, const entt::entity& e);
	void add_to_tile(int x, int y, const entt::entity& e);
	void remove_from_tile(int x, int y, const entt::entity& e);
	void render(entt::registry& reg, graphics_context& context, static_sprite_dic& dic);
	void update(entt::registry& reg, SDL_Keycode);
	bool is_entity(int x, int y, const entt::entity& ent);
	bool in_range(int x, int y);
	bool passable(int x, int y);


	static void register_events(entt::registry& reg);
};



