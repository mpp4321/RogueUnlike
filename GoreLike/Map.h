#pragma once

#include <vector>
#include <unordered_map>
#include "entt.hpp"

/*

	MapComponent is a component which holds a vector of unordered_map of tiles which hold unique identifiers of the entities which belong to the map
		component

	Entities which belong to a MapComponent must contain a WorldPositionComponent
		and may contain a NonPassable component
*/

#define TILE_SIZE 32

/*
	Component struct which tags non-passable entities
*/
struct NonPassable {
};

struct WorldPositionComponent;

class MapComponent
{
private:
	std::unordered_set<entt::entity> global_set;
	std::vector<std::unordered_set<entt::entity>> tiles;
	size_t _width, _height;
	entt::registry* _reg;
public:
	MapComponent(entt::registry& reg, size_t width, size_t height);
	~MapComponent();
	size_t width();
	size_t height();
	const std::unordered_set<entt::entity>& get_tile(int x, int y);
	const std::unordered_set<entt::entity>& all_entities() const;
	entt::entity create_map_entity(int x, int y);
	void set_entity_world_position(int x, int y, const entt::entity& e);
	void add_to_tile(int x, int y, const entt::entity& e);
	void remove_from_tile(int x, int y, const entt::entity& e);
	bool is_entity(const entt::entity& ent);
	bool is_entity(int x, int y, const entt::entity& ent);
};

struct MapRendererFlag {};

