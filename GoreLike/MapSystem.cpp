#include "MapSystem.h"

MapSystem::MapSystem(map_data&& map_component, entt::registry& reg, entt::entity player, unsigned int width, unsigned int height, unsigned int player_x, unsigned int player_y) : _player_id(player)
{
	entt::entity map_component_entity = reg.create();
	//We only call create_map_entity here in the initialization of the map so we are safe to add map_component_entity
	mc_handle = entt::handle(reg, map_component_entity);

	if(width > 0 && height > 0) 
		map_component.tiles[ player_y*width + player_x ].entities.insert(_player_id);

	reg.emplace<map_data>(map_component_entity, map_component);
}

void MapSystem::on_destroy(entt::registry & reg, entt::entity ent)
{
	auto& mdata = reg.get<map_data>(ent);
	for (const auto& elem : mdata.tiles) {
		for (const auto& ent : elem.entities) {
			reg.destroy(ent);
		}
	}
}

void MapSystem::render(entt::registry& reg, graphics_context& context, static_sprite_dic& dic) {

	auto& mc = mc_handle.get<map_data>();

	int c = 0;
	for (auto& tile : mc.tiles) {
		if (tile.id.has_value()) {
			int x = TILE_SIZE * (c % mc.height);
			int y = TILE_SIZE * (c / mc.height);
			sprite_render::render(
				{ tile.id.value() },
				{ x, y },
				dic,
				context
			);
		}
		c++;
	}
	
}

#define MOVEMENT(x, y) \
	for (auto entity : reg.view<world_position_controllable, world_position>()) {\
		movement(reg, entity, x, y);\
	}\

void MapSystem::update(entt::registry& reg, SDL_Keycode polled_key)
{
	static auto movement = [this](entt::registry& reg, entt::entity e, int dx, int dy) {
		auto& wp = reg.get<world_position>(e);
		int n_x = wp.x + dx;
		int n_y = wp.y + dy;
		if( in_range(n_x, n_y) && passable(n_x, n_y) ) {
			set_entity_world_position(wp.x + dx, wp.y + dy, e);
		}
	};

	//Update code
	if (polled_key == SDLK_d) {
		MOVEMENT(1, 0);
	}

	if (polled_key == SDLK_a) {
		MOVEMENT(-1, 0);
	}

	if (polled_key == SDLK_w) {
		MOVEMENT(0, -1);
	}

	if (polled_key == SDLK_s) {
		MOVEMENT(0, 1);
	}
}

void MapSystem::on_position_update(entt::registry& reg, entt::entity e)
{
	auto* mapSystemPtr = reg.ctx<MapSystem*>();
	auto& c1 = reg.get<world_position>(e);
	auto& c2 = reg.get<old_world_position>(e);
	mapSystemPtr->remove_from_tile(c2.x, c2.y, e);
	mapSystemPtr->add_to_tile(c1.x, c1.y, e);
	reg.patch<old_world_position>(e, [&c1](auto& owp) {
		owp.x = c1.x;
		owp.y = c1.y;
	});
}

const Tile& MapSystem::get_tile(unsigned int x, unsigned int y)
{
	auto& mc = mc_handle.get<map_data>();
	return mc.tiles[y * mc.width + x];
}

entt::entity MapSystem::create_map_entity(entt::registry& reg, unsigned int x, unsigned int y)
{
	entt::entity ent = reg.create();
	reg.emplace<world_position>(ent, world_position { x, y });
	reg.emplace<old_world_position>(ent, x, y);
	reg.emplace<screen_transform>(ent, screen_transform{ static_cast<int>(x) * TILE_SIZE, static_cast<int>(y) * TILE_SIZE });
	return ent;
}

entt::entity MapSystem::create_map_entity(entt::registry& reg, unsigned int x, unsigned int y, std::string id)
{
	auto ent = create_map_entity(reg, x, y);
	reg.emplace<static_sprite>(ent, static_sprite{ id });
	return ent;
}

void MapSystem::load_by_map_data(map_data&& map_component, unsigned int player_x, unsigned int player_y) {
	auto& reg = *mc_handle.registry();
	entt::entity map_component_entity = reg.create();
	mc_handle.destroy();
	mc_handle = entt::handle(reg, map_component_entity);
	auto& width = map_component.width;
	map_component.tiles[ player_y*width + player_x ].entities.insert(_player_id);
	reg.emplace<map_data>(map_component_entity, map_component);
}

void MapSystem::load_static_map_by_id(const std::string& id, unsigned int player_start_x, unsigned int player_start_y)
{
	auto tmxMap = tmx::Map();
	auto mapDir = std::filesystem::current_path().append(fmt::format("resources\\maps\\{}.tmx", id)).string();
	tmxMap.load( mapDir );
	map_data && mapd = tiled_map_util::initialize_map(*mc_handle.registry(), tmxMap);
	unsigned int w = mapd.width, h = mapd.height;
	load_by_map_data(std::forward<map_data>(mapd), player_start_x, player_start_y);
}

void MapSystem::set_entity_world_position(unsigned int x, unsigned int y, const entt::entity& e)
{
	entt::registry* _reg = mc_handle.registry();
	auto& worldPos = _reg->get<world_position>(e);
	_reg->patch<world_position>(e, [&x, &y](auto& wpc) {
		wpc.x = x;
		wpc.y = y;
	});
	if (_reg->has<screen_transform>(e)) {
		_reg->patch<screen_transform>(e, [&x, &y](auto& st) {
			st.transform_x = TILE_SIZE * x;
			st.transform_y = TILE_SIZE * y;
		});
	}
}

void MapSystem::outside_no_update_set_entity_world_position(entt::registry& reg, unsigned int x, unsigned int y, const entt::entity& e)
{
	world_position& worldPos = reg.get<world_position>(e);
	worldPos.x = x;
	worldPos.y = y;
	if (reg.has<screen_transform>(e)) {
		reg.patch<screen_transform>(e, [&x, &y](auto& st) {
			st.transform_x = TILE_SIZE * x;
			st.transform_y = TILE_SIZE * y;
		});
	}
}

void MapSystem::add_to_tile(unsigned int x, unsigned int y, const entt::entity& e)
{
	if (!is_entity(x, y, e)) {
		mc_handle.patch<map_data>([&x, &y, &e](auto& mc) {
			mc.tiles[y * mc.width + x].entities.insert(e);
		});
	}
	else {
		//Entity already exists in the world.
	}
}

void MapSystem::remove_from_tile(unsigned int x, unsigned int y, const entt::entity& e)
{
	if (is_entity(x, y, e)) {
		mc_handle.patch<map_data>([&x, &y, &e](auto& mc) {
			mc.tiles[y * mc.width + x].entities.erase(e);
		});
	}
	else {
		//Entity does not exist in global set does not exist in any subset
	}
}

bool MapSystem::is_entity(unsigned int x, unsigned int y, const entt::entity& ent)
{
	auto& mc = mc_handle.get<map_data>();
	auto& tile = mc.tiles[y * mc.width + x];
	return tile.entities.count(ent);
}

bool MapSystem::in_range(int x, int y)
{
	auto& wc = mc_handle.get<map_data>();
	return x >= 0 && y >= 0 && x < wc.width && y < wc.height;
}

bool MapSystem::passable(unsigned int x, unsigned int y)
{
	auto& t = get_tile(x, y);
	return t.isPassable && t.entities.size() == 0;
}

void MapSystem::register_events(entt::registry& reg)
{
	reg.on_update<world_position>().connect<&MapSystem::on_position_update>();
	reg.on_destroy<map_data>().connect<&MapSystem::on_destroy>();
}
