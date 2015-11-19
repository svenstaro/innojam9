#ifndef GAMEOVERSTATEHPP
#define GAMEOVERSTATEHPP

#include "game.hpp"

#include "states/menu_state.hpp"
#include "components/component_textitem.hpp"

#include "entityx/entityx.h"

#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

void game_over_back(Game* game){
    game->popstate();
    game->popstate();
}

class GameOverState : public MenuState {
  public:
    GameOverState(Game *game, float score) : MenuState(game), m_score(score) {
    };
  protected:
    void init_menuitems(int w, int h){
        entityx::Entity background = m_entities.create();
        background.assign<Drawable>("menu_background", 800, 800);
        background.assign<Position>(glm::vec2(0.f, 0.f));

        entityx::Entity title = m_entities.create();
        title.assign<Drawable>("menu_game_over", 200, 800);
        title.assign<Position>(glm::vec2(0.f, 80.f));

        entityx::Entity score = m_entities.create();
        score.assign<TextItem>("Score: " + std::to_string(int(m_score)), "font20", SDL_Color{162,157,196,255});
        score.assign<Position>(glm::vec2(350, 493));

        entityx::Entity btn_back = m_entities.create();
        btn_back.assign<Drawable>("menu_game_over_back", 64, 64, 0, AnimTemplate(64, 64));
        btn_back.assign<Position>(glm::vec2(64, 472));
        btn_back.assign<MenuItem>("back", game_over_back);

    }
    float m_score;
};

#endif
