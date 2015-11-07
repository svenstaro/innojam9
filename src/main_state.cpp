#include "main_state.hpp"

#include "functions_pathing.hpp"

#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_path.hpp"
#include "component_player.hpp"
#include "component_moving.hpp"
#include "component_light.hpp"
#include "system_collision.hpp"
#include "system_controls.hpp"
#include "system_draw.hpp"
#include "system_movement.hpp"
#include "system_path.hpp"
#include "system_highscore.hpp"
#include "system_emitter.hpp"
#include "system_orbspawn.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>
#include <glm/gtc/constants.hpp>

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
    m_systems.add<EmitterSystem>(linear_path, 0.5, 0.3);
    m_systems.add<OrbSpawnSystem>(&m_entities);
    m_systems.configure();

    entityx::Entity player = m_entities.create();
    //must be at (r, 3/2pi) !!
    player.assign<Position>(glm::vec2(100.f, 1.5 * glm::pi<double>()));
    player.assign<Moving>(200.f);
    player.assign<Collidable>(15);
    player.assign<Drawable>("player", 30, 30, 10);
    player.assign<Player>();
    player.assign<Light>("gradient");

    entityx::Entity background = m_entities.create();
    background.assign<Position>(glm::vec2(0.f, 0.f));
    background.assign<Drawable>("wood", 1000, 1000, 0);

    entityx::Entity outer_bound = m_entities.create();
    outer_bound.assign<Position>(glm::vec2(0.f, 0.f));
    outer_bound.assign<Drawable>("outer_bound", 600, 600, 1);

    AnimTemplate fire_anim(32, 32, 6, 0, 10);
    entityx::Entity fire = m_entities.create();
    fire.assign<Position>(glm::vec2(0.f, 0.f));
    fire.assign<Drawable>("fire", 100, 100, 1, fire_anim);
    fire.assign<Light>("gradient");

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
            else if(e.key.keysym.sym == SDLK_F3) {
                m_game->toggle_debug_mode();
            }
        }
    }

    m_systems.update<DrawSystem>(dt);
    m_systems.update<ControlSystem>(dt);
    m_systems.update<CollisionSystem>(dt);
    m_systems.update<MovementSystem>(dt);
    m_systems.update<HighscoreSystem>(dt);
    m_systems.update<OrbSpawnSystem>(dt);
    m_systems.update<PathSystem>(dt);
    m_systems.update<EmitterSystem>(dt);
}
