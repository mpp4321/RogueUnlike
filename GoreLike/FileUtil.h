#pragma once

#include "SDL.h"
#include <string>
#include <filesystem>

class static_sprite_dic;
	
void resource_tree_search_and_add(static_sprite_dic* dict, const std::filesystem::path& path, bool recursive=false);

std::string get_stem(const std::string& str);

std::string get_resource_dir();

