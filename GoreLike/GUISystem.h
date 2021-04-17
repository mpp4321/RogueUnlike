#pragma once

#include "SystemClasses.h"
#include "FileUtil.h"
#include "SDL_ttf.h"

class GUISystem {
	
public:
	TTF_Font* font;


	GUISystem() {
		font = NULL;
	}

	~GUISystem() {
		if (font) {
			TTF_CloseFont(font);
		}
	}

	entt::entity create_button(entt::registry& reg, std::string res, screen_transform t, rectangle_bound b, EH_DELEGATE_TYPE cb) {
		auto e = reg.create();
		reg.emplace<screen_transform>(e, t);
		reg.emplace<rectangle_bound>(e, b);
		reg.emplace<button>(e, cb);
		reg.emplace<static_sprite>(e, static_sprite{ res });
		return e;
	}
	
	void initialize() {
		
		//Initialize TTF
		if (TTF_Init() != 0) {
			std::cout << "Error initializing SDL_TTF " << SDL_GetError() << std::endl;
		}
		else {
			std::cout << "Initialized SDL_TTF Successfully " << std::endl;
		}

		auto s = get_resource_dir().append("fonts\\ksink.ttf");
		font = TTF_OpenFont(s.c_str(), 8);

		if (!font) {
			std::cout << TTF_GetError() << std::endl;
		}
	}

	void render(entt::registry& reg, graphics_context& context, static_sprite_dic& dic) {
		for (auto m_ent : reg.view<options_menu_gui>()) {
			options_menu_gui& m = reg.get<options_menu_gui>(m_ent);
			for (auto ent : m.gui_text_components) {
				screen_transform st = reg.has<screen_transform>(ent) ? reg.get<screen_transform>(ent) : screen_transform{};
				gui_text& gui_t = reg.get<gui_text>(ent);
				
				SDL_Surface* messagePtr;

				if (gui_t.message) messagePtr = gui_t.message;
				else messagePtr = TTF_RenderText_Solid(font, gui_t.text_line.c_str(), gui_t.color);

				context.draw_image({ st.transform_x, st.transform_y, messagePtr->w, messagePtr->h }, messagePtr);
			}
		}
	};

	bool intersects(rectangle_bound b, screen_transform t, std::pair<int, int> p) {
		int& x = t.transform_x;
		int& y = t.transform_y;
		if (p.first >= x && p.second >= y && p.first < (x + b.width) && p.second < (y + b.height)) {
			return true;
		}
		return false;
	}

	void update(entt::registry& reg, c_input_event ev) {

		if (ev.mouse_clicked) {
			std::pair<int, int> mouse_loc = ev.mouse_info;
			auto v = reg.view<rectangle_bound, screen_transform, button>();
			for (auto e : v) {
				button& b = reg.get<button>(e);
				if (intersects(reg.get<rectangle_bound>(e), reg.get<screen_transform>(e), mouse_loc)) {
					b.callback({ reg, e });
				}
			}
		}

	};


};

