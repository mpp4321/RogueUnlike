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

struct world_position_controllable {};

struct bump_event {
	//Function which is called when an entity bumps into owning entity
	//Params are entity which bumped this one
	entt::delegate<void(entt::entity)> call_back;
};

struct old_world_position {
	old_world_position(size_t _x, size_t _y) : x(_x), y(_y) {};
    size_t x, y;
};

struct world_position {
	world_position(size_t _x, size_t _y) : x(_x), y(_y) {};
    size_t x, y;
};

//Flag to render a worldcomponent
struct world_render {};

struct screen_transform {
	int transform_x, transform_y;
};

//Basically "single sprite Renderable"
struct static_sprite {
	std::string id;
};

class static_sprite_dic {
private:
	std::unordered_map<std::string, SDL_Surface*> texture_map;
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
		auto iter = texture_map.begin();
		std::advance(iter, RandomUtil::Get()->random_int(0, texture_map.size() - 1));
		return (*iter).second;
	}

	SDL_Surface* get_texture(std::string id) {
		return texture_map[id];
	}

	SDL_Surface* load_texture(std::string path, std::string id) {
		SDL_Surface* loaded_texture = IMG_Load(path.c_str());
		if (loaded_texture) {
			texture_map[id] = loaded_texture;
		}
		return loaded_texture;
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
	void draw_image(SDL_Rect& dest, SDL_Surface* img);
	void update();
	void clear();

	~graphics_context() {
		free_resources();
	}
};

