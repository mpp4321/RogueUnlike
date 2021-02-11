#pragma once

#include <typeindex>
#include <unordered_map>
#include <string>
#include <any>
#include <optional>
#include "SystemClasses.h"
#include "entt.hpp"
#include "json.hpp"
#include "FileUtil.h"

using namespace entt;
using json = nlohmann::json;

template< class T >
class JsonFactory {
public:
	static void initialize(json j, registry& reg, entity ent) {
		T u{};
		u.from_json(j, u);
		reg.emplace<T>(ent, u);
	}
};

class world_position_Factory : public JsonFactory<world_position> {};

using FactoryMapType = std::unordered_map<std::string, delegate<void(json, registry&, entity)>>;

class entity_json_utils
{

public:
	static entity initialize_entity(registry& reg, const std::string& jpath) {
		std::string fileContents = get_file_contents(jpath);
		json j = json::parse(fileContents);
		/*
		
		[
			{
			"type": "typeName",
			"object": {some object data}
			},
		] 
		
		*/
		entity x = reg.create();

		for (json inner : j) {
			get_type_map()[inner["type"].get<std::string>()]( inner["object"], reg, x );
		}
		return x;
	}
private:

	
	/* unordered map of string to struct types for component initialization */
	static FactoryMapType  get_type_map() {
		static bool initialized = false;

		static FactoryMapType struct_type_map{};

		if (initialized) {
			return struct_type_map;
		}
		else {
			initialized = true;
			//Initialize here
			#define S(key, type) struct_type_map.insert_or_assign(key, delegate<void(json, registry&, entity)> { entt::connect_arg<&JsonFactory<type>::initialize> });
			S("world_position", world_position);

			return struct_type_map;
		}
	}
	
};

