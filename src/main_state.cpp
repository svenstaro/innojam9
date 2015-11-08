#include "main_state.hpp"

#include "functions_pathing.hpp"

#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_path.hpp"
#include "component_player.hpp"
#include "component_moving.hpp"
#include "component_velocity.hpp"
#include "component_light.hpp"
#include "system_collision.hpp"
#include "system_controls.hpp"
#include "system_draw.hpp"
#include "system_movement.hpp"
#include "system_path.hpp"
#include "system_highscore.hpp"
#include "system_emitter.hpp"
#include "system_orbspawn.hpp"
#include "system_runes.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include <SDL.h>

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

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
    m_systems.add<MovementSystem>(RING_INNER, RING_OUTER);
    m_systems.add<HighscoreSystem>(m_game);
    m_systems.add<EmitterSystem>(m_game);
    m_systems.add<RunesSystem>();
    m_systems.add<OrbSpawnSystem>(m_game, m_entities, RING_INNER, RING_OUTER);
    //
    //     // glm::vec2 origin = glm::vec2(300, 0);
    //     // auto parable = create_parable(origin, glm::vec2(300, glm::half_pi<float>()), glm::vec2(glm::one_over_root_two<float>()*300, glm::quarter_pi<float>()));
    //
    //     // glm::vec2 origin = glm::vec2(150, 0);
    //     // auto parable = create_parable(origin, glm::vec2(300, glm::half_pi<float>()), glm::vec2(150, glm::pi<float>()));
    //     //
    //     // m_systems.add<EmitterSystem>(m_game, parable, origin, 1, 10, 0.3);
    //     m_systems.add<EmitterSystem>(m_game, linear_path, 0.5, 0.3);

    m_systems.configure();

    float hp;
    switch(m_game->difficulty()) {
        default:
        case EASY:
            hp = 10;
            break;
        case MEDIUM:
            hp = 5;
            break;
        case HARD:
            hp = 3;
            break;
        case SVENSTARO:
            hp = 0;
            break;
    }

    entityx::Entity player = m_entities.create();
    // must be at (r, 3/2pi) !!
    player.assign<Position>(
            glm::vec2((RING_OUTER - RING_INNER) / 2.0 + RING_INNER, 1.5 * glm::pi<double>()));
    player.assign<Velocity>();
    player.assign<Collidable>(15);
    player.assign<Drawable>("player", 50, 30, 10, AnimTemplate(15, 25, 4, 0, 6));
    player.assign<Player>(hp);
    player.assign<Light>("gradient");

    entityx::Entity background = m_entities.create();
    background.assign<Position>(glm::vec2(0.f, 0.f));
    background.assign<Drawable>("wood", 1000, 1000, 0);

    entityx::Entity outer_bound = m_entities.create();
    outer_bound.assign<Position>(glm::vec2(0.f, 0.f));
    // for the outer bound we need additional 50 radius,
    // so that the player is inside the circle.
    outer_bound.assign<Drawable>("outer_bound", 2 * (int)RING_OUTER + 100,
            2 * (int)RING_OUTER + 100, 1);

    AnimTemplate fire_anim(32, 32, 6, 0, 10);
    entityx::Entity fire = m_entities.create();
    fire.assign<Position>(glm::vec2(0.f, 0.f));
    fire.assign<Drawable>("fire", 100, 100, 1, fire_anim);
    fire.assign<Light>("gradient");

    Mix_VolumeMusic(50);
    Mix_PlayMusic(m_game->res_manager().music("music1"), -1);

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
            } else if (e.key.keysym.sym == SDLK_F3) {
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
    m_systems.update<RunesSystem>(dt);
}
