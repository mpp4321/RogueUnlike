#pragma once

#include "SDL2/SDL.h"
#include <string>
#include <filesystem>

class static_sprite_dic;
	
void resource_tree_search_and_add(static_sprite_dic* dict, const std::filesystem::path& path, bool recursive=false);

std::string get_stem(const std::string& str);

std::string get_file_contents(const std::string& path);

std::filesystem::path get_resource_dir();

