#pragma once

#include <vector>
#include "entt.hpp"

/*

	Map is a collection (Vector) of Tiles which contain a resource location (id) which matches up to a static sprite dictionary
	Tiles contain a Vector of entities relying on that resource location
	All entitiy data is a entt::entity type

*/

struct Tile {
	Tile(std::string resource_id) : resource_id(resource_id) {
	}
	std::string resource_id = "NULL";
	bool isWalkable = true;
};

class MapComponent
{
private:
	std::vector<Tile*> tiles;
	std::unordered_set<entt::entity> entities;
	size_t _width, _height;
public:
	MapComponent(size_t width, size_t height);
	~MapComponent();
	size_t width();
	size_t height();
	Tile* get_tile(int x, int y);
	void set_tile(int x, int y, Tile* t);
	bool is_entity(const entt::entity& ent);
};

struct MapRendererFlag {};

