#ifndef GAMEOVERSTATEHPP
#define GAMEOVERSTATEHPP

#include "game.hpp"

#include "strapon/state/state.hpp"

#include "entityx/entityx.h"

#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif


class GameOverState : public State {
  public:
    GameOverState(Game *game) : m_game(game) {
    };
    ~GameOverState() {
    };
    int init() override {
        return 0;
    };git
    void update(double dt) override {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                m_game->shutdown();
            }
            if(e.type == SDL_KEYDOWN) {
                m_game->exit();
            }
        }
    };

  private:
    Game *m_game;
};

#endif
