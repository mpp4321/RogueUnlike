#pragma once

#include "Map.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "FileUtil.h"

class tiled_map_util {
public:
	static map_data&& initialize_map(tmx::Map& m);
};
