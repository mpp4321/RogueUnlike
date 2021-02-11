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
		unsigned int width, unsigned int height,
		unsigned int player_x, unsigned int player_y);
	static void on_destroy(entt::registry& reg, entt::entity ent);
	static void on_position_update(entt::registry & reg, entt::entity ent);
	const Tile& get_tile(unsigned int x, unsigned int y);
	static entt::entity create_map_entity(entt::registry& reg, unsigned int x, unsigned int y);
	/* create_map_entity based on some abstract "id" will load based on json soon*/
	static entt::entity create_map_entity(entt::registry& reg, unsigned int x, unsigned int y, std::string id);
	void load_by_map_data(map_data&& map_component, unsigned int player_x, unsigned int player_y);
	void load_static_map_by_id(const std::string& id, unsigned int player_start_x, unsigned int player_start_y);
	void set_entity_world_position(unsigned int x, unsigned int y, const entt::entity& e);
	void add_to_tile(unsigned int x, unsigned int y, const entt::entity& e);
	void remove_from_tile(unsigned int x, unsigned int y, const entt::entity& e);
	void render(entt::registry& reg, graphics_context& context, static_sprite_dic& dic);
	void update(entt::registry& reg, SDL_Keycode);
	bool is_entity(unsigned int x, unsigned int y, const entt::entity& ent);
	bool in_range(int x, int y);
	bool passable(unsigned int x, unsigned int y);


	static void register_events(entt::registry& reg);
};



