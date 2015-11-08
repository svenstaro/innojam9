#ifndef DIFFICULTYMENU_STATE_MAIN_HPP
#define DIFFICULTYMENU_STATE_MAIN_HPP

#include "menu_state.hpp"
#include "main_state.hpp"

#include "entityx/entityx.h"

#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_menuitem.hpp"

#include "anim_template.hpp"

#include "game.hpp"
#include "difficulty.hpp"

#include <glm/vec2.hpp>
#include <iostream>

void start_game(Game* game){
	//game->popstate();
	game->states().push({"main", std::make_unique<MainState>(game)});
	game->states().top().second->init();
}

void set_easy(Game* game){
	game->set_difficulty(EASY);
	start_game(game);
}

void set_medium(Game* game){
	game->set_difficulty(MEDIUM);
	start_game(game);
}

void set_hard(Game* game){
	game->set_difficulty(HARD);
	start_game(game);
}

void set_svenstaro(Game* game){
	game->set_difficulty(SVENSTARO);
	start_game(game);
}

void back(Game* game){
	game->popstate();
}

class DifficultyMenuState : public MenuState {
public:
	DifficultyMenuState(Game *game): MenuState(game){}

protected:
	void init_menuitems(int w, int h){
		std::cout << "init difficulty menu" << std::endl;
		float yoff = 500;

		entityx::Entity background = m_entities.create();
		background.assign<Drawable>("menu_red_background", 800, 800, 0);
		background.assign<Position>(glm::vec2(0.f, 0.f));

		// entityx::Entity title = m_entities.create();
		// title.assign<Drawable>("title", w, h);
		// title.assign<Position>(glm:vec(0.f, 0.f));

		entityx::Entity btn_easy = m_entities.create();
		btn_easy.assign<Drawable>("difficulty_easy", 32, 128, 0, AnimTemplate(128, 32, 1, 0));
		btn_easy.assign<Position>(glm::vec2(w/8-64, yoff));
		btn_easy.assign<MenuItem>("easy", set_easy);

		entityx::Entity btn_medium = m_entities.create();
		btn_medium.assign<Drawable>("difficulty_medium", 32, 128, 0, AnimTemplate(128, 32, 1, 0));
		btn_medium.assign<Position>(glm::vec2(3*w/8-64, yoff));
		btn_medium.assign<MenuItem>("medium", set_medium);

		entityx::Entity btn_hard= m_entities.create();
		btn_hard.assign<Drawable>("difficulty_hard", 32, 128, 0, AnimTemplate(128, 32, 1, 0));
		btn_hard.assign<Position>(glm::vec2(5*w/8-64, yoff));
		btn_hard.assign<MenuItem>("hard", set_hard);

		entityx::Entity btn_svenstaro = m_entities.create();
		btn_svenstaro.assign<Drawable>("difficulty_svenstaro", 32, 128, 0, AnimTemplate(128, 32, 1, 0));
		btn_svenstaro.assign<Position>(glm::vec2(7*w/8-64, yoff));
		btn_svenstaro.assign<MenuItem>("svenstaro", set_svenstaro);


		entityx::Entity btn_back = m_entities.create();
		btn_back.assign<Drawable>("menu_back", 32, 32, 0, AnimTemplate(64, 64, 1, 0));
		btn_back.assign<Position>(glm::vec2(128, 400));
		btn_back.assign<MenuItem>("back", back);
	}
};

#endif
