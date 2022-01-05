#include "GameStates.h" 
#define INIT_MAP_WIDTH 5
#define INIT_MAP_HEIGHT 5

TestGameState::TestGameState(entt::registry& reg) 
{

	auto player_ent = MapSystem::create_map_entity(reg, 0, 0, "angel");
	reg.emplace<world_position_controllable>(player_ent);

	reg.on_construct<timed>().connect<entt::invoke<&timed::init_call_backs>>();
	reg.on_construct<game_timed>().connect<entt::invoke<&game_timed::init_call_backs>>();


	this->gui_system = {};
	this->gui_system.initialize();
	//this->gui_system.create_button(reg, "button_test", { 0,0 }, { 128, 128 },
	//	EH_DELEGATE_TYPE ( [](const void* p, entt::handle h) {} )
	//);


	this->map_system = MapSystem(map_data{}, reg, player_ent, 0, 0, 0, 0);
	reg.set<MapSystem*>(&map_system);
	MapSystem::register_events(reg);
	map_system.load_static_map_by_id("map1", 0, 0);

	this->_context.initialize_graphics();

    auto resource_path = "resources/dcssf/";
    resource_tree_search_and_add(&get_sprite_dict(), std::filesystem::current_path().append(resource_path), true);

	//Test code

	//auto e = reg.create();
	//reg.emplace<gui_text>(e, gui_text{ "Sick dog bro" });
	{
		auto e = reg.create();
		reg.emplace<screen_transform>(e, screen_transform { 0,0 });
		sdl_gui::create_gui_options_menu(reg, e, { "Test 1", "Test 2", "Test 3" }, {
			{[](const void* p, entt::handle h) {}},
			{[](const void* p, entt::handle h) {}},
			{[](const void* p, entt::handle h) {}}
		}, 0);
	}

	//End test code

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

Uint64 TestGameState::calculate_and_update_dt() 
{
	time_last = time_now;
	time_now = SDL_GetPerformanceCounter();
	return (float)((time_now - time_last) * 1000.0f / (float)SDL_GetPerformanceFrequency());
}

void TestGameState::start(registry& reg)
{
	SDL_Event e;
	while (!quit) {
		polled_key = SDLK_INSERT;
		SDL_GetMouseState(&mouse_loc.first, &mouse_loc.second);

		// Only tick time if we poll an event
		while (SDL_PollEvent(&e) != 0) {
			update(reg, &e);
		}

		float dt = calculate_and_update_dt();
		tick(reg, dt);

		render(reg);
	}
}

void TestGameState::tick(registry& reg, float dt) {
	
	auto anim_sprite_view = reg.view<timed, animated_sprite>();
	for (entt::entity ent : anim_sprite_view) {
		bool anim_sprite = false;
		reg.patch<timed>(ent, [&](auto& timer) {
			timer.increment_time({ reg, ent }, dt);
		});
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
	gui_system.render(reg, _context, _sprite_dict);

	_context.update();
}


void TestGameState::update(registry& reg, SDL_Event* ev)
{

	if (ev->type == SDL_QUIT) {
		quit = true;
		return;
	} else if (ev->type == SDL_KEYDOWN) {
		auto key = ev->key.keysym.sym;
		polled_key = key;
		//KeyCode stuff here
		if (polled_key == SDLK_q)
		{
			quit = true;
			return;
		}

		map_system.update(reg, polled_key);

		//Temporary test for game_timed
		size_t player_speed = 1;
		size_t other_speed = 1;
		
		//Needs some type of speed to have a game-timed component
		for (auto ent : reg.view<game_timed/*, entity_stats*/>()) {
			reg.patch<game_timed>(ent, [&](auto& timer) {
				timer.increment_time({ reg, ent }, other_speed, player_speed);
			});
		}
	}
	else if (ev->type == SDL_MOUSEBUTTONDOWN) {
		gui_system.update(reg, c_input_event{polled_key, ev->type == SDL_MOUSEBUTTONDOWN, mouse_loc});
	}
}
