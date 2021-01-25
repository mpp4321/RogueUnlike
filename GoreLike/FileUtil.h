#pragma once

#include "SDL.h"
#include <string>
#include <filesystem>

class StaticSpriteDictionary;
	
void resource_tree_search_and_add(StaticSpriteDictionary* dict, const std::filesystem::path& path, bool recursive=false);

