#include "main_state.hpp"

#include "functions_pathing.hpp"

#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_path.hpp"
#include "component_player.hpp"
#include "component_moving.hpp"
#include "system_collision.hpp"
#include "system_controls.hpp"
#include "system_draw.hpp"
#include "system_movement.hpp"
#include "system_path.hpp"
#include "system_highscore.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

MainState::MainState(Game *game) : m_game(game) {
}

MainState::~MainState() {
}

int MainState::init() {
    m_systems.add<DrawSystem>(m_game);
    m_systems.add<ControlSystem>();
    m_systems.add<CollisionSystem>();
    m_systems.add<PathSystem>();
    m_systems.add<MovementSystem>(50, 300);
    m_systems.add<HighscoreSystem>();
    m_systems.configure();

    entityx::Entity player = m_entities.create();
    player.assign<Position>(glm::vec2(100.f, 0.f));
    player.assign<Moving>(100.f);
    player.assign<Drawable>("player", 80, 80, 10);
    player.assign<Player>();

    entityx::Entity background = m_entities.create();
    background.assign<Position>(glm::vec2(0.f, 0.f));
    background.assign<Drawable>("dirt", 2000, 2000);

    entityx::Entity fire = m_entities.create();
    fire.assign<Position>(glm::vec2(0.f, 0.f));
    fire.assign<Drawable>("fire", 100, 100);

    entityx::Entity test = m_entities.create();
    test.assign<Position>(glm::vec2(40.f, 225.f));
    test.assign<Path>(linear_path, glm::vec2(1.f,45.f),20.f);
    test.assign<Moving>(0.5f);
    test.assign<Drawable>("player", 20 , 20);
    return 0;
}


void MainState::update(double dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            m_game->shutdown();
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                m_game->shutdown();
            }
        }
    }

    m_systems.update<DrawSystem>(dt);
    m_systems.update<ControlSystem>(dt);
    m_systems.update<CollisionSystem>(dt);
    m_systems.update<MovementSystem>(dt);
    m_systems.update<HighscoreSystem>(dt);
}
