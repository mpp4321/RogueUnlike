#include "GameStates.h"

#define INIT_MAP_WIDTH 5
#define INIT_MAP_HEIGHT 5

TestGameState::TestGameState(entt::registry& reg) /*:*/
	//map_system(MapSystem(
	//	map_data{ std::vector<Tile> {}, INIT_MAP_WIDTH, INIT_MAP_HEIGHT },
	//	reg, INIT_MAP_WIDTH, INIT_MAP_HEIGHT))
{
	auto tmxMap = tmx::Map();
	auto mapDir = std::filesystem::current_path().append("resources\\maps\\map1.tmx").u8string();
	tmxMap.load(mapDir);
	map_data && mapd = tiled_map_util::initialize_map(tmxMap);
	size_t w = mapd.width, h = mapd.height;
	this->map_system = MapSystem(std::forward<map_data>(mapd), reg, w, h);
	this->_context.initialize_graphics();

    auto resource_path = "resources/dcssf/";
    //auto resource_path = "resources/dcssf/monster";
    //auto resource_path_tiles = "resources/dcssf/dungeon/floor";

    resource_tree_search_and_add(&get_sprite_dict(), std::filesystem::current_path().append(resource_path), true);
    //resource_tree_search_and_add(&get_sprite_dict(), std::filesystem::current_path().append(resource_path_tiles));

    //get_sprite_dict().print_loaded_textures();

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
		auto& ssc = reg.get<static_sprite>(ent);
		auto& transform = reg.get<screen_transform>(ent);
		SDL_Surface* imgsurf = this->_sprite_dict.get_texture(ssc.id);
		if (!imgsurf) continue;
		SDL_Rect&& rect = SDL_Rect{ transform.transform_x, transform.transform_y, imgsurf->w, imgsurf->h };
		_context.draw_image(rect, imgsurf);
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
