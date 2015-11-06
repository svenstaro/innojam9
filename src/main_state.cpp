#include "main_state.hpp"

#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_moving.hpp"
#include "system_collision.hpp"
#include "system_controls.hpp"
#include "system_draw.hpp"
#include "system_movement.hpp"

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
    m_systems.add<MovementSystem>();
    m_systems.configure();

    entityx::Entity player = m_entities.create();
    player.assign<Position>(glm::vec2(0.f, 0.f));
    player.assign<Moving>(10.f);
    player.assign<Drawable>("player", 80, 80);

    entityx::Entity fire = m_entities.create();
    fire.assign<Position>(glm::vec2(0.f, 0.f));
    fire.assign<Drawable>("fire", 100, 100);

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
}
