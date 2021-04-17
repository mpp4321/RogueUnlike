#pragma once

#include "SystemClasses.h"

namespace sdl_gui {
	
	static void menu_destroy(entt::registry& r, entt::entity e) {
		r.get<options_menu_gui>(e);
	}

	inline void register_events(entt::registry& r) {

		r.on_destroy<options_menu_gui>().connect<&menu_destroy>();
	}

	inline SDL_Keycode from_char(const char& i) {
		if ('a' <= i && 'z' >= i) {
			auto v = i - 'a';
			return SDLK_a + v;
		}
		return 0;
	}

	inline entt::entity create_gui_text(entt::registry& r, const std::string& t, std::optional<screen_transform> tr = {}) {
		auto e = r.create();
		r.emplace<gui_text>(e, gui_text{ t });
		if (tr) {
			r.emplace<screen_transform>(e, tr.value());
		}
		return e;
	}

	//Loads requirements for gui_options_menu onto entt::entity e
	inline entt::entity create_gui_options_menu(entt::registry& r, entt::entity e, std::vector<std::string> options, std::vector<EH_DELEGATE_TYPE> callbacks, int8_t priority) {
		//Menu location
		screen_transform& t = r.get<screen_transform>(e);
		std::vector<entt::entity> v = {};
		for (size_t i = 0; i < options.size(); i++) {
			std::string s = options[i];
			int tx = t.transform_x + 32;
			int ty = t.transform_y + 32 + (i * 32);
			v.push_back(create_gui_text(r, s, { screen_transform {tx, ty} }));
		}
		std::unordered_map < SDL_Keycode, EH_DELEGATE_TYPE > call_backs{};
		size_t c = 0;
		for (char z = 'a'; z <= 'z'; z++) {
			if (c >= callbacks.size()) {
				break;
			}
			call_backs.emplace(sdl_gui::from_char(z), callbacks.at(c));
			c++;
		}
		
		r.emplace<options_menu_gui>(e, options_menu_gui{ v, call_backs, priority });

		return e;
	}

}

