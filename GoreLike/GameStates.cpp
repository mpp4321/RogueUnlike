#include "GameStates.h" 
#define INIT_MAP_WIDTH 5
#define INIT_MAP_HEIGHT 5

TestGameState::TestGameState(entt::registry& reg) 
{

	auto player_ent = MapSystem::create_map_entity(reg, 0, 0, "angel");
	reg.emplace<world_position_controllable>(player_ent);

	this->map_system = MapSystem(map_data{}, reg, player_ent, 0, 0, 0, 0);
	reg.set<MapSystem*>(&map_system);
	MapSystem::register_events(reg);
	map_system.load_static_map_by_id("map1", 0, 0);

	this->_context.initialize_graphics();

    auto resource_path = "resources/dcssf/";
    resource_tree_search_and_add(&get_sprite_dict(), std::filesystem::current_path().append(resource_path), true);

	start(reg);
}

TestGameState::~TestGameState()
{
	_context.free_resources();
}

static_sprite_dic& TestGameState::get_sprite_dict()
{
	return _sprite_dict;
}

void TestGameState::start(registry& reg)
{
	SDL_Event e;
	while (!quit) {
		polled_key = SDLK_INSERT;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
				break;
			} 
			if (e.type == SDL_KEYDOWN) {
				auto key = e.key.keysym.sym;
				polled_key = key;
				update(reg, 0.0f);
			}
		}
		render(reg);
	}
}

void TestGameState::render(registry& reg)
{

	//RENDER systems here
	_context.clear();

	map_system.render(reg, _context, _sprite_dict);

	for (auto ent : reg.view<static_sprite, screen_transform>()) {
		auto[ssc, transform] = reg.get<static_sprite, screen_transform>(ent);
		sprite_render::render(ssc, transform, _sprite_dict, _context);
	}
	_context.update();
}


void TestGameState::update(registry& reg, float dt)
{
	if (polled_key == SDLK_q)
	{
		quit = true;
		return;
	}
	
	map_system.update(reg, polled_key);
}
