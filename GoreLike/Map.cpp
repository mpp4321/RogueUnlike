#include "Map.h"
#include "SystemClasses.h"

MapComponent::MapComponent(entt::registry& reg, size_t width, size_t height) : _width(width), _height(height), _reg(&reg)
{
	tiles.reserve(width * height);
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			entt::entity creating = create_map_entity(j, i);
			auto& ssc = reg.emplace<StaticSpriteComponent>(creating);
			ssc.id = "brown_ooze";
			tiles.push_back(std::unordered_set<entt::entity>{ creating });
			global_set.insert(creating);
		}
	}

}

MapComponent::~MapComponent()
{
	printf(
		"Deconstructor in MapComponent\n"
	);
	for (auto ent : global_set) {
		_reg->destroy(ent);
	}
}

size_t MapComponent::width()
{
	return this->_width;
}

size_t MapComponent::height()
{
	return this->_height;
}

const std::unordered_set<entt::entity>& MapComponent::get_tile(int x, int y)
{
	return tiles[y*width() + x];
}

const std::unordered_set<entt::entity>& MapComponent::all_entities() const
{
	return this->global_set;
}

entt::entity MapComponent::create_map_entity(int x, int y)
{
	entt::entity ent = _reg->create();
	_reg->emplace<WorldPositionComponent>(ent, x, y);
	_reg->emplace<ScreenTransform>(ent, x*TILE_SIZE, y*TILE_SIZE);
	return ent;
}

void MapComponent::set_entity_world_position(int x, int y, const entt::entity& e)
{
	auto& worldPos = _reg->get<WorldPositionComponent>(e);
	remove_from_tile(worldPos.x, worldPos.y, e);
	add_to_tile(x, y, e);
	_reg->patch<WorldPositionComponent>(e, [&x, &y](auto& wpc) {
		wpc.x = x;
		wpc.y = y;
	});
	if (_reg->has<ScreenTransform>(e)) {
		_reg->patch<ScreenTransform>(e, [&x, &y](auto& st) {
			st.transform_x = TILE_SIZE * x;
			st.transform_y = TILE_SIZE * y;
		});
	}
}

void MapComponent::add_to_tile(int x, int y, const entt::entity& e)
{
	if (!is_entity(e)) {
		tiles[y * width() + x].insert(e);
		global_set.insert(e);
	}
	else {
		//Entity already exists in the world.
	}
}

void MapComponent::remove_from_tile(int x, int y, const entt::entity& e)
{
	if (is_entity(e)) {
		tiles[y * width() + x].erase(e);
		global_set.erase(e);
	}
	else {
		//Entity does not exist in global set does not exist in any subset
	}
}

bool MapComponent::is_entity(const entt::entity& ent)
{
	return global_set.count(ent);
}

bool MapComponent::is_entity(int x, int y, const entt::entity& ent)
{
	return tiles[y*width() + x].count(ent);
}

