#include "MapSystem.h"

MapSystem::MapSystem(map_data&& map_component, entt::registry& reg, size_t width, size_t height)
{

	entt::entity map_component_entity = reg.create();
	//We only call create_map_entity here in the initialization of the map so we are safe to add map_component_entity
	mc_handle = entt::handle(reg, map_component_entity);
	
	//for (int i = 0; i < height; i++) {
	//	for (int j = 0; j < width; j++) {
	//		//entt::entity creating = create_map_entity(j, i);
	//		//auto& ssc = reg.emplace<static_sprite>(creating);
	//		//ssc.id = "brown_ooze";
	//		map_component.tiles.push_back(Tile{ std::optional("cobble_blood_3_old"), std::unordered_set<entt::entity> {}, true });
	//	}
	//}

	{
		entt::entity creating = create_map_entity(0, 0);
		auto& ssc = reg.emplace<static_sprite>(creating);
		reg.emplace<world_position_controllable>(creating);
		ssc.id = "angel";
		map_component.tiles[0].entities.insert(creating);
	}

	reg.emplace<map_data>(map_component_entity, map_component);
	
	mc_handle.registry()->on_update<world_position>().connect<&MapSystem::on_position_update>(this);
}

void MapSystem::render(entt::registry& reg, graphics_context& context, static_sprite_dic& dic) {

	auto& mc = mc_handle.get<map_data>();

	int c = 0;
	for (auto& tile : mc.tiles) {
		if (tile.id.has_value()) {
			SDL_Surface* imgsurf = dic.get_texture(*tile.id);
			if (!imgsurf) continue;
			int x = TILE_SIZE * (c % mc.height);
			int y = TILE_SIZE * (c / mc.height);
			SDL_Rect&& rect = SDL_Rect{ x, y, imgsurf->w, imgsurf->h };
			context.draw_image(rect, imgsurf);
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
	auto& c1 = reg.get<world_position>(e);
	auto& c2 = reg.get<old_world_position>(e);
	remove_from_tile(c2.x, c2.y, e);
	add_to_tile(c1.x, c1.y, e);
	reg.patch<old_world_position>(e, [&c1](auto& owp) {
		owp.x = c1.x;
		owp.y = c1.y;
	});
}

const Tile& MapSystem::get_tile(int x, int y)
{
	auto& mc = mc_handle.get<map_data>();
	return mc.tiles[y * mc.width + x];
}

entt::entity MapSystem::create_map_entity(int x, int y)
{
	entt::registry* _reg = mc_handle.registry();
	entt::entity ent = _reg->create();
	_reg->emplace<world_position>(ent, x, y);
	_reg->emplace<old_world_position>(ent, x, y);
	_reg->emplace<screen_transform>(ent, x*TILE_SIZE, y*TILE_SIZE);
	return ent;
}

entt::entity MapSystem::create_map_entity(int x, int y, std::string id)
{
	entt::registry* _reg = mc_handle.registry();
	entt::entity ent = _reg->create();
	_reg->emplace<world_position>(ent, x, y);
	_reg->emplace<old_world_position>(ent, x, y);
	_reg->emplace<screen_transform>(ent, x*TILE_SIZE, y*TILE_SIZE);
	_reg->emplace<static_sprite>(ent, static_sprite{ id });

	return ent;
}

void MapSystem::set_entity_world_position(int x, int y, const entt::entity& e)
{
	entt::registry* _reg = mc_handle.registry();
	auto& worldPos = _reg->get<world_position>(e);
	remove_from_tile(worldPos.x, worldPos.y, e);
	add_to_tile(x, y, e);
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

void MapSystem::add_to_tile(int x, int y, const entt::entity& e)
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

void MapSystem::remove_from_tile(int x, int y, const entt::entity& e)
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

bool MapSystem::is_entity(int x, int y, const entt::entity& ent)
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

bool MapSystem::passable(int x, int y)
{
	auto& t = get_tile(x, y);
	return t.isPassable && t.entities.size() == 0;
}
