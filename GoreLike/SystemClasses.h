#pragma once

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 608

#include "RandomUtil.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include "SDL.h"
#include "SDL_image.h"
#include "entt.hpp"
#include <future>
#include "json.hpp"

struct world_position_controllable {};

//TODO make this json compatible
// Simply make different "components" for different bump events like bump_event_enemy, bump_event_npc etc
struct bump_event {
	//Function which is called when an entity bumps into owning entity
	//Params are entity which bumped this one
	entt::delegate<void(entt::entity)> call_back;
};

struct __time_data {
	float curTime = 0.0f;
	float maxTime = 0.0f;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(__time_data, maxTime)

struct timed {
	using identifier = std::string;
	std::unordered_map<identifier, __time_data> times{};

	void initialize_time(identifier id, float maxTime) {
		times[id] = { maxTime, maxTime };
	}

	bool increment_time(identifier id, float dt) {
		if (times.count(id)) {
			auto& [curTime, maxTime] = times[id];
			curTime -= dt;
			if (curTime < 0.001f) {
				curTime = maxTime;
				return true;
			}
			return false;
		}
		return false;
	}

};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(timed, times)

struct old_world_position {
    unsigned int x = 0, y = 0;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(old_world_position, x, y)

struct world_position {
    unsigned int x = 0, y = 0;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(world_position, x, y)


//Flag to render a worldcomponent
struct world_render {};

struct screen_transform {
	int transform_x, transform_y;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(screen_transform, transform_x, transform_y)

//Basically "single sprite Renderable"
struct static_sprite {
	std::string id = "brown_ooze";
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(static_sprite, id)

struct animated_sprite {
	static std::string id; /* defined in cpp */

	size_t c_sprite_pos = 0;
	std::vector<static_sprite> sprite_sequence{};

	static_sprite get_cur() const {
		return sprite_sequence.at(c_sprite_pos);
	}

	void next() {
		c_sprite_pos = (c_sprite_pos + 1) % sprite_sequence.size();
	}
	
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(animated_sprite, sprite_sequence)

class static_sprite_dic {
private:
	std::unordered_map<std::string, SDL_Surface*> texture_map;

	std::vector<std::future<void>> _futures;
	static std::mutex mu_texture;

	void _async_load_image(std::string path, std::string id) {
		//SDL_Surface* loaded_texture = IMG_Load(path.c_str());
		SDL_Surface* loaded_texture = IMG_Load(path.c_str());
		std::lock_guard<std::mutex> load_lock(mu_texture);
		if (loaded_texture) {
			texture_map[id] = loaded_texture;
		}
	};

public:
	static_sprite_dic() {
	}

	~static_sprite_dic() {
		for (auto [strin, surface] : texture_map) {
			SDL_FreeSurface(surface);
		}
	}
	
	auto get_internal_map() {
		return texture_map;
	}

	void print_loaded_textures() {
		for (auto [strin, surface] : texture_map) {
			printf("%s ", strin.c_str());
		}
		printf("\n");
	}

	SDL_Surface* random_texture() {
		std::lock_guard<std::mutex> mu_guard{ mu_texture };

		auto iter = texture_map.begin();
		std::advance(iter, RandomUtil::Get()->random_int(0, texture_map.size() - 1));
		return (*iter).second;
	}

	//May return nullptr if texture is not yet loaded.
	SDL_Surface* const get_texture(std::string id) const {
		std::lock_guard<std::mutex> mu_guard{ mu_texture };

		if (texture_map.count(id) == 0) {
			return nullptr;
		}
		return texture_map.at(id);
	}

	/* Threadsafe load_texture */
	void load_texture(std::string path, std::string id) {
		_futures.push_back( 
			std::async(std::launch::async, &static_sprite_dic::_async_load_image, this, path, id));
	}

};

class sprite_sheet {
private:
	SDL_Surface* image_src;
	SDL_Rect image_rect;

public:

	sprite_sheet(const char* path, int width, int height) {
		image_src = SDL_LoadBMP(path);
		image_rect.w = image_src->w / width;
		image_rect.h = image_src->h / height;
	}
	~sprite_sheet() {
		SDL_FreeSurface(image_src);
	}

	void draw_sprite(int x, int y, int ix, int iy, SDL_Surface* dest) {
		image_rect.x = ix;
		image_rect.y = iy;
		SDL_Rect r;
		r.x = x;
		r.y = y;
		SDL_BlitSurface(image_src, &image_rect, dest, &r);
	}

};

class graphics_context {
public:
	SDL_Window* window;
	SDL_Surface* windowSurface;

	void free_resources();
	void initialize_graphics();
	void draw_image(SDL_Rect& dest, SDL_Surface* img) const;
	void update();
	void clear();

	~graphics_context() {
		free_resources();
	}
};

namespace sprite_render {

	static void render(
		const static_sprite& ssc,
		const screen_transform& transform,
		const static_sprite_dic& dict,
		const graphics_context& context) {

		SDL_Surface* imgsurf = dict.get_texture(ssc.id);
		if (!imgsurf) return;
		SDL_Rect&& rect = SDL_Rect{ transform.transform_x, transform.transform_y, imgsurf->w, imgsurf->h };
		context.draw_image(rect, imgsurf);

	};

};
