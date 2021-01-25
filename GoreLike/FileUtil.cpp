#include "FileUtil.h"
#include "SystemClasses.h"

void resource_tree_search_and_add(StaticSpriteDictionary* dict, const std::filesystem::path& path, bool recursive)
{
	for (auto& p : std::filesystem::directory_iterator(path)) {
		if (p.is_directory() && recursive) {
			resource_tree_search_and_add(dict, p.path(), recursive);
		}
		else {
			if (p.path().has_extension() && p.path().extension() == ".png") {
				dict->load_texture(p.path().u8string(), p.path().stem().u8string());
			}
		}
	}
}

