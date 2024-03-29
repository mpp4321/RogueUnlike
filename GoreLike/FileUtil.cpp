#include "FileUtil.h"
#include "SystemClasses.h"

#include <iterator>
#include <fstream>

void resource_tree_search_and_add(static_sprite_dic* dict, const std::filesystem::path& path, bool recursive)
{
	for (auto& p : std::filesystem::directory_iterator(path)) {
		if (p.is_directory() && recursive) {
			std::cout << "Found new directory: " << p.path().string() << std::endl;
			resource_tree_search_and_add(dict, p.path(), recursive);
		}
		else {
			if (p.path().has_extension() && p.path().extension() == ".png") {
				//test_futures.push_back(std::async(std::launch::async, &static_sprite_dic::load_texture, *dict, p.path().u8string(), p.path().stem().u8string()));
				dict->load_texture(p.path().string(), p.path().stem().string());
			}
		}
	}
}

std::string get_stem(const std::string& str)
{
	return std::filesystem::path(str).stem().string();
}

std::string get_file_contents(const std::string& path)
{
	auto k = std::ifstream(std::filesystem::path(path).string(), std::ios::in);
	return { std::istreambuf_iterator<char>(k), std::istreambuf_iterator<char>() };
}

std::filesystem::path get_resource_dir()
{
	auto path = std::filesystem::current_path() / std::filesystem::path("resources");
	return path;
}

