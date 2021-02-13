#pragma once

#include "MapComponents.h"
#include "MapSystem.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "FileUtil.h"
#include "entity_json_utils.h"

class tiled_map_util {
public:
	static map_data initialize_map(entt::registry& reg, tmx::Map& m);
};
