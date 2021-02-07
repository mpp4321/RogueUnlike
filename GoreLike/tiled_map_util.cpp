#include "tiled_map_util.h"

map_data tiled_map_util::initialize_map(tmx::Map& m)
{
    map_data&& d{};

    auto tiles = m.getLayers().begin();

    auto& tile_set = m.getTilesets()[0];

    auto& tile_layer = *tiles;
    tiles++;

    auto& entity_layer = *tiles;
    
    d.tiles.reserve(static_cast<size_t>(m.getTileCount().x) * static_cast<size_t>(m.getTileCount().y));
    d.width = m.getTileCount().x;
    d.height = m.getTileCount().y;

	auto& vec_tiles = tile_layer->getLayerAs<tmx::TileLayer>().getTiles();
	for (size_t y = 0; y < m.getTileCount().y; y++) {
		for (size_t x = 0; x < m.getTileCount().x; x++) {
            size_t pos = y * m.getTileCount().x + x;
            std::string path = tile_set.getTiles()[vec_tiles[pos].ID].imagePath;
            d.tiles.push_back(Tile{ std::optional(get_stem(path)), {} });
            //TODO make it load entities too
        }
    }

    return d;
}
