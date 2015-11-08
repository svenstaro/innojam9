#ifndef MAINMENU_STATE_MAIN_HPP
#define MAINMENU_STATE_MAIN_HPP

#include "menu_state.hpp"
#include "difficultymenu_state.hpp"

#include "entityx/entityx.h"

#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_menuitem.hpp"

#include "anim_template.hpp"

#include "game.hpp"

#include <glm/vec2.hpp>

void new_game(Game* game){
	//open difficulty menu first
	game->states().push({"difficulty", std::make_unique<DifficultyMenuState>(game)});
	game->states().top().second->init();
}

void show_highscore(Game* game){
	std::cout << "highscore" << std::endl;
}

void shutdown(Game* game){
	game->shutdown();
}

class MainMenuState : public MenuState {
public:
	MainMenuState(Game *game): MenuState(game){}

protected:
	void init_menuitems(int w, int h){
		float yoff = 200;

		entityx::Entity background = m_entities.create();
		background.assign<Drawable>("menu_red_background", 800, 800, 0);
		background.assign<Position>(glm::vec2(0.f, 0.f));

		// entityx::Entity title = m_entities.create();
		// title.assign<Drawable>("title", w, h);
		// title.assign<Position>(glm:vec(0.f, 0.f));

		entityx::Entity btn_game = m_entities.create();
		btn_game.assign<Drawable>("menu_newgame", 64, 256, 0, AnimTemplate(256, 64, 1, 0));
		btn_game.assign<Position>(glm::vec2(w/2-128, yoff + 100.f));
		btn_game.assign<MenuItem>("new_game", new_game);

		entityx::Entity btn_highscore = m_entities.create();
		btn_highscore.assign<Drawable>("menu_highscore", 64, 256, 0, AnimTemplate(256, 64, 1, 0));
		btn_highscore.assign<Position>(glm::vec2(w/2-128, yoff + 200.f));
		btn_highscore.assign<MenuItem>("highscore", show_highscore);

		entityx::Entity btn_exit = m_entities.create();
		btn_exit.assign<Drawable>("menu_exit", 64, 256, 0, AnimTemplate(256, 64, 1, 0));
		btn_exit.assign<Position>(glm::vec2(w/2-128, yoff + 300.f));
		btn_exit.assign<MenuItem>("exit", shutdown);
	}
};

#endif
