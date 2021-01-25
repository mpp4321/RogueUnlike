#include "Map.h"

MapComponent::MapComponent(size_t width, size_t height) : _width(width), _height(height)
{
	tiles.reserve(width * height);
	
	for (int i = 0; i < width * height; i++) {
		tiles[i] = new Tile("dirt_0_old");
	}

}

MapComponent::~MapComponent()
{
	for (auto p : tiles)
		delete p;
}

size_t MapComponent::width()
{
	return this->_width;
}

size_t MapComponent::height()
{
	return this->_height;
}

Tile* MapComponent::get_tile(int x, int y)
{
	return tiles[y*width() + x];
}

void MapComponent::set_tile(int x, int y, Tile* t)
{
	size_t pos = y * width() + x;
	if (tiles[pos]) {
		delete tiles[pos];
		tiles[pos] = nullptr;
		tiles[pos] = t;
	}
}

bool MapComponent::is_entity(const entt::entity& ent)
{
	return entities.count(ent);
}

