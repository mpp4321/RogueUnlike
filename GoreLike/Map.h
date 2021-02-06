#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include "entt.hpp"

/*

	MapComponent is a component which holds a vector of unordered_map of tiles which hold unique identifiers of the entities which belong to the map
		component

	Entities which belong to a MapComponent must contain a WorldPositionComponent
		and may contain a NonPassable component
*/

#define TILE_SIZE 32

/*

	Tiles are a set of entities which exist on that tile and a solid flag to show if the tile can be moved onto

*/
struct Tile {
	std::optional<std::string> id;
    std::unordered_set<entt::entity> entities;
    bool isPassable = true;
};

struct world_position;

/*

MapComponent, a unique-component which exists under a handle within the MapSystem

	- Contains data on what entities are where under "tiles" array as well as a width and height size

*/
class map_data
{
public:
	std::vector<Tile> tiles;
	size_t width, height;
};

struct MapRendererFlag {};

