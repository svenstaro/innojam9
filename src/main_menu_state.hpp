#ifndef MAINMENU_STATE_MAIN_HPP
#define MAINMENU_STATE_MAIN_HPP

#include "menu_state.hpp"
#include "difficultymenu_state.hpp"

#include "entityx/entityx.h"

#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_textitem.hpp"
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

		entityx::Entity title = m_entities.create();
		title.assign<Drawable>("menu_title", 256, 512);
		title.assign<Position>(glm::vec2((800 - 512) / 2, 0.f));

		entityx::Entity btn_game = m_entities.create();
		btn_game.assign<Drawable>("menu_newgame", 64, 256, 0, AnimTemplate(256, 64, 1, 0));
		btn_game.assign<Position>(glm::vec2(w/2-128, yoff + 100.f));
		btn_game.assign<MenuItem>("new_game", new_game);

		// entityx::Entity btn_highscore = m_entities.create();
		// btn_highscore.assign<Drawable>("menu_highscore", 64, 256, 0, AnimTemplate(256, 64, 1, 0));
		// btn_highscore.assign<Position>(glm::vec2(w/2-128, yoff + 200.f));
		// btn_highscore.assign<MenuItem>("highscore", show_highscore);

		entityx::Entity btn_exit = m_entities.create();
		btn_exit.assign<Drawable>("menu_exit", 64, 256, 0, AnimTemplate(256, 64, 1, 0));
		btn_exit.assign<Position>(glm::vec2(w/2-128, yoff + 200.f));
		btn_exit.assign<MenuItem>("exit", shutdown);

		SDL_Color color = SDL_Color{107,86,86,255};
		int credit_x = 60;
		int credit_y = 450;

		entityx::Entity credits1 = m_entities.create();
		credits1.assign<Position>(glm::vec2(credit_x, credit_y));
		credits1.assign<TextItem>("Alexander Dmitriev", "font20", color);

		entityx::Entity credits2 = m_entities.create();
		credits2.assign<Position>(glm::vec2(credit_x, credit_y + 25));
		credits2.assign<TextItem>("Andra Ruebsteck", "font20", color);

		entityx::Entity credits3 = m_entities.create();
		credits3.assign<Position>(glm::vec2(credit_x, credit_y + 50));
		credits3.assign<TextItem>("Frederick Gnodtke", "font20", color);

		entityx::Entity credits4 = m_entities.create();
		credits4.assign<Position>(glm::vec2(credit_x, credit_y+75));
		credits4.assign<TextItem>("Oliver Heidmann", "font20", color);

		entityx::Entity credits5 = m_entities.create();
		credits5.assign<Position>(glm::vec2(credit_x, credit_y+100));
		credits5.assign<TextItem>("Sven-Hendrik Haase", "font20", color);

		//------------------------------------------------

		int music_credit_x = 430;
		int music_credit_y = 450;

		entityx::Entity credits6 = m_entities.create();
		credits6.assign<Position>(glm::vec2(music_credit_x, music_credit_y+50));
		credits6.assign<TextItem>("Music fron: http://opengameart.org/", "font20", color);

		entityx::Entity credits7 = m_entities.create();
		credits7.assign<Position>(glm::vec2(music_credit_x, music_credit_y+75));
		credits7.assign<TextItem>("Space Boss Battle Theme", "font20", color);

		entityx::Entity credits8 = m_entities.create();
		credits8.assign<Position>(glm::vec2(music_credit_x, music_credit_y+100));
		credits8.assign<TextItem>("Pocket Destroyer", "font20", color);
	}
};

#endif
