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

    auto ent = entity_json_utils::initialize_entity(reg, "black_ooze");

	map_system.set_entity_world_position(2, 2, ent);
	

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

		time_last = time_now;
		time_now = SDL_GetPerformanceCounter();
		float dt = (float)((time_now - time_last) * 1000.0f / (float)SDL_GetPerformanceFrequency());
		tick(reg, dt);

		render(reg);
	}
}

void TestGameState::tick(registry& reg, float dt) {
	
	auto anim_sprite_view = reg.view<timed, animated_sprite>();

	for (entt::entity ent : anim_sprite_view) {

		bool anim_sprite = false;
		reg.patch<timed>(ent, [&](auto& timer) {
			if (timer.increment_time(animated_sprite::id, dt)) {
				reg.patch<animated_sprite>(ent, [](auto& x) { x.next(); });
				const animated_sprite& a_sprite_ref = reg.get<animated_sprite>(ent);
				reg.emplace_or_replace<static_sprite>(ent, a_sprite_ref.get_cur());
			}
		});
		
		auto timz = reg.get<timed>(ent);

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
