#ifndef GAMEOVERSTATEHPP
#define GAMEOVERSTATEHPP

#include "game.hpp"

#include "menu_state.hpp"

#include "entityx/entityx.h"
#include "strapon/sdl_helpers/sdl_helpers.hpp"

#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif


class GameOverState : public MenuState {
  public:
    GameOverState(Game *game) : MenuState(game) {
    };
  protected:
    void init_menuitems(int w, int h){
        entityx::Entity background = m_entities.create();
        background.assign<Drawable>("menu_background", 800, 800);
        background.assign<Position>(glm::vec2(0.f, 0.f));

        entityx::Entity title = m_entities.create();
        title.assign<Drawable>("menu_game_over", 64, 256);
        title.assign<Position>(glm::vec2(0.f, 0.f));
    }
};

#endif
