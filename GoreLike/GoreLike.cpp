// GoreLike.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "entt.hpp"
#include "SystemClasses.h"
#include "SDL.h"
#include "FileUtil.h"
#include <iostream>

int main(int argc, char* args[])
{
    entt::registry reg;
    auto ent = reg.create();

    GraphicsContext context;
    context.initialize_graphics();
    StaticSpriteDictionary dict;
    auto resource_path = "resources/dcssf/monster";
    resource_tree_search_and_add(&dict, std::filesystem::current_path().append(resource_path));

    dict.print_loaded_textures();
    
    int cur_x = 0;
    int cur_y = 0;
    for (auto [_, surf] : dict.get_internal_map()) {
        SDL_Surface* t_surf = dict.random_texture();
        SDL_Rect&& t_rect = SDL_Rect{ cur_x, cur_y, t_surf->w, t_surf->h };
        context.draw_image(t_rect, t_surf);

        cur_x += t_surf->w;
        if (cur_x > SCREEN_WIDTH) {
            cur_x = 0;
            cur_y += t_surf->h;
        }
        if (cur_y > SCREEN_HEIGHT) {
            break;
        }
    }

    context.update();
    std::cin.get();
    context.free_resources();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
