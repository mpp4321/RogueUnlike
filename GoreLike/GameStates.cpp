#include "GameStates.h"

TestGameState::TestGameState()
{
}

TestGameState::~TestGameState()
{
	_context.free_resources();
}

StaticSpriteDictionary& TestGameState::get_sprite_dict()
{
	return _sprite_dict;
}

void TestGameState::start(registry& reg)
{
	this->_context.initialize_graphics();

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

	//resource_tree_search_and_add(&this->_sprite_dict, "resources/");
}

void TestGameState::render(registry& reg)
{
	//TODO add these to simple functions

	//maybe this can be update code lol
	//for (auto ent : reg.view<MapComponent>()) {
	//	const MapComponent& mc = reg.get<MapComponent>(ent);
	//	for (auto _ent : mc.all_entities()) {
	//		if (reg.has < StaticSpriteComponent, WorldPositionComponent >(_ent)) {
	//			auto& wpc = reg.get<WorldPositionComponent>(_ent);
	//			reg.patch<StaticSpriteComponent>(_ent, [this, &wpc](auto& ssc) {
	//				SDL_Surface* surf = _sprite_dict.get_texture(ssc.id);
	//				ssc.transform_x = wpc.x * surf->w;
	//				ssc.transform_y = wpc.y * surf->h;
	//			});
	//		}
	//	}
	//}

	//RENDER systems here
	_context.clear();
	for (auto ent : reg.view<StaticSpriteComponent, ScreenTransform>()) {
		auto& ssc = reg.get<StaticSpriteComponent>(ent);
		auto& transform = reg.get<ScreenTransform>(ent);
		SDL_Surface* imgsurf = this->_sprite_dict.get_texture(ssc.id);
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

	static auto lmbd_update = [](registry& reg, int dw, int dh) {
		auto mcv = reg.view<MapComponent>();
		reg.destroy(mcv.begin(), mcv.end());

		entt::entity newMapComponent = reg.create();
		reg.emplace<MapComponent>(newMapComponent, reg, dw, dh);
	};


	//Update code
	if (polled_key == SDLK_d) {
		for (auto z : reg.view<MapComponent>()) {
			auto& mc = reg.get<MapComponent>(z);
			lmbd_update(reg, mc.width() + 1, mc.height());
			break;
		}
	}

	if (polled_key == SDLK_a) {
		for (auto z : reg.view<MapComponent>()) {
			auto& mc = reg.get<MapComponent>(z);
			lmbd_update(reg, mc.width() - 1, mc.height());
			break;
		}
	}

	if (polled_key == SDLK_w) {
		for (auto z : reg.view<MapComponent>()) {
			auto& mc = reg.get<MapComponent>(z);
			lmbd_update(reg, mc.width(), mc.height() + 1);
			break;
		}
	}

	if (polled_key == SDLK_a) {
		for (auto z : reg.view<MapComponent>()) {
			auto& mc = reg.get<MapComponent>(z);
			lmbd_update(reg, mc.width(), mc.height() - 1);
			break;
		}
	}
}
