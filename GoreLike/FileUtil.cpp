#include "FileUtil.h"
#include "SystemClasses.h"

void resource_tree_search_and_add(static_sprite_dic* dict, const std::filesystem::path& path, bool recursive)
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

std::string get_stem(const std::string& str)
{
	return std::filesystem::path(str).stem().u8string();
}

std::string get_resource_dir()
{
	return std::filesystem::current_path().u8string();
}

