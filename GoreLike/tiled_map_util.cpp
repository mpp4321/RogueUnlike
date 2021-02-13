#include "tiled_map_util.h"


/* maps global id to local tilesheet id*/
int convert_tile_id(int id, const std::vector<tmx::Tileset>& tileset, int index) {
    for (int i = 0; i < index; i++) {
        auto size = tileset.at(i).getTiles().size();
        if (id < size) return -1;
        id -= size;
    }
    //I have no idea if -index is right but I just need to subtract one for the entity layer
    return id-index;
}

map_data tiled_map_util::initialize_map(entt::registry& reg, tmx::Map& m)
{
    map_data&& d{};

    auto& tile_set = m.getTilesets()[0];
    auto& ent_tile_set = m.getTilesets()[1];
    
    d.tiles.reserve(static_cast<size_t>(m.getTileCount().x) * static_cast<size_t>(m.getTileCount().y));
    d.width = m.getTileCount().x;
    d.height = m.getTileCount().y;

	auto& tile_layer = m.getLayers().at(0);
	auto& vec_tiles = tile_layer->getLayerAs<tmx::TileLayer>().getTiles();
	for (size_t y = 0; y < m.getTileCount().y; y++) {
		for (size_t x = 0; x < m.getTileCount().x; x++) {
            size_t pos = y * m.getTileCount().x + x;
			auto tiles = m.getLayers().begin();
            tiles++;

            auto ent_set = std::unordered_set<entt::entity>{};
            for (; tiles != m.getLayers().end(); tiles++) {
                auto& ent_layer = *tiles;
                auto& layer_cast = ent_layer->getLayerAs<tmx::TileLayer>();

                auto id = convert_tile_id(layer_cast.getTiles().at(pos).ID, m.getTilesets(), 1);
                if (id == -1) continue;
                auto& properties = ent_tile_set.getTiles().at(id).properties;
                if (properties.size() > 0) {
                    std::optional<std::string> foundEJsonPath = {};
                    for (auto prop : properties) {
                        if (prop.getName() == "EJsonPath") {
                            foundEJsonPath.emplace(prop.getStringValue());
                            break;
                        }
                    }

                    if (foundEJsonPath) {
						auto ent = entity_json_utils::initialize_entity(reg, *foundEJsonPath);

                        MapSystem::outside_no_update_set_entity_world_position(reg, x, y, ent);

                        ent_set.insert(ent);
                    }
                }
            }
            std::string path = tile_set.getTiles()[vec_tiles[pos].ID].imagePath;
            d.tiles.push_back(Tile{ std::optional(get_stem(path)), ent_set });
            //TODO make it load entities too
        }
    }

    return d;
}
