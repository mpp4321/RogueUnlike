// GoreLike.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "entt.hpp"
#include "SystemClasses.h"
#include "SDL.h"
#include "FileUtil.h"
#include "GameStates.h"
#include "Map.h"
#include <iostream>

int main(int argc, char* args[])
{
    entt::registry reg;
    MapComponent wmc{ reg, 3, 3 };
    { auto e = reg.create(); reg.emplace<MapComponent>(e, wmc); }
    TestGameState gs{};

    auto resource_path = "resources/dcssf/monster";
    resource_tree_search_and_add(&gs.get_sprite_dict(), std::filesystem::current_path().append(resource_path));
    gs.get_sprite_dict().print_loaded_textures();

    gs.start(reg);
    //GraphicsContext context;
    //context.initialize_graphics();
    //StaticSpriteDictionary dict;
    
    //int cur_x = 0;
    //int cur_y = 0;
    //for (auto [_, surf] : dict.get_internal_map()) {
    //    SDL_Surface* t_surf = dict.random_texture();
    //    SDL_Rect&& t_rect = SDL_Rect{ cur_x, cur_y, t_surf->w, t_surf->h };
    //    context.draw_image(t_rect, t_surf);

    //    cur_x += t_surf->w;
    //    if (cur_x > SCREEN_WIDTH) {
    //        cur_x = 0;
    //        cur_y += t_surf->h;
    //    }
    //    if (cur_y > SCREEN_HEIGHT) {
    //        break;
    //    }
    //}

    return 0;
}
