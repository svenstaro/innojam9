#include "main_state.hpp"

#include "functions_pathing.hpp"

#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_path.hpp"
#include "component_player.hpp"
#include "component_moving.hpp"
#include "component_emitter.hpp"
#include "component_ring.hpp"
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
#include "system_life_time.hpp"
#include "system_sound.hpp"

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
    m_systems.add<CollisionSystem>(this);
    m_systems.add<PathSystem>();
    m_systems.add<MovementSystem>(RING_INNER, RING_OUTER);
    m_systems.add<HighscoreSystem>(this);
    m_systems.add<EmitterSystem>();
    m_systems.add<RunesSystem>();
    m_systems.add<OrbSpawnSystem>(m_game, m_entities, RING_INNER, RING_OUTER);
    m_systems.add<LifeTimeSystem>();
    m_systems.add<SoundSystem>(&m_game->res_manager());
    //
    //     // glm::vec2 origin = glm::vec2(300, 0);
    //     // auto parable = create_parable(origin, glm::vec2(300, glm::half_pi<float>()),
    //     glm::vec2(glm::one_over_root_two<float>()*300, glm::quarter_pi<float>()));
    //
    //     // glm::vec2 origin = glm::vec2(150, 0);
    //     // auto parable = create_parable(origin, glm::vec2(300, glm::half_pi<float>()),
    //     glm::vec2(150, glm::pi<float>()));
    //     //
    //     // m_systems.add<EmitterSystem>(m_game, parable, origin, 1, 10, 0.3);
    //     m_systems.add<EmitterSystem>(m_game, linear_path, 0.5, 0.3);

    m_systems.configure();

    float hp;
    switch (m_game->difficulty()) {
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
    player.assign<Collidable>(5.f);
    player.assign<Drawable>("player", 50, 30, 10, AnimTemplate(15, 25, 4, 0, 6));
    player.assign<Player>(hp);
    player.assign<Light>("gradient");

    entityx::Entity background = m_entities.create();
    background.assign<Position>(glm::vec2(0.f, 0.f));
    background.assign<Drawable>("wood", 1000, 1000, 0);

    entityx::Entity outer_bound = m_entities.create();
    outer_bound.assign<Position>(glm::vec2(0.f, 0.f));
    outer_bound.assign<Ring>();
    // for the outer bound we need additional 50 radius,
    // so that the player is inside the circle.
    outer_bound.assign<Drawable>("outer_bound", 2 * (int)RING_OUTER + 100,
                                 2 * (int)RING_OUTER + 100, 2);
    Mix_VolumeMusic(50);
    Mix_PlayMusic(m_game->res_manager().music("music1"), -1);
    // Setting order of levels
    m_level_vector = {Level::LEVEL_ONE(),   Level::LEVEL_TWO(),  Level::LEVEL_THREE(),
                      Level::LEVEL_FOUR(),  Level::LEVEL_FIVE(), Level::LEVEL_SIX(),
                      Level::LEVEL_SEVEN(), Level::LEVEL_EIGHT()};
    load_level(0);
    return 0;
}

void MainState::update_level() {
    //rumble_for(0.75f);
    m_current_level_index++;
    if (m_current_level_index == m_level_vector.size()) {
        // TODO: GAMEOVER STUFF;
    }
    clear_level();

    m_number_of_collected_orbs = 0;
    m_number_if_needed_orbs = m_level_vector[m_current_level_index].m_orbs_to_next_level;
}

void MainState::load_level(unsigned int level_index) {
    AnimTemplate fire_anim(32, 32, 6, 0, 10);
    entityx::Entity fire = m_entities.create();
    fire.assign<Position>(glm::vec2(0.f, 0.f));
    fire.assign<Drawable>("fire", 100, 100, 1, fire_anim);
    fire.assign<Light>("gradient");
    fire.assign<Emitter>(m_level_vector[level_index]);
}
void MainState::clear_level() {
    entityx::ComponentHandle<Path> path;
    entityx::ComponentHandle<Emitter> emitter;
    for (entityx::Entity entity : m_entities.entities_with_components(path, emitter)) {
        entity.destroy();
    }
}

unsigned int MainState::get_current_level_index() {
    return m_current_level_index;
}

unsigned int MainState::get_max_level_index() {
    return m_level_vector.size() - 1;
}

Level MainState::get_current_level() {
    return m_level_vector[m_current_level_index];
}

void MainState::update_orb_count()
{
    m_orbs_collected++;
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
    if(m_number_of_collected_orbs == m_number_if_needed_orbs)
    {
        update_level(); 
    }
    m_systems.update<DrawSystem>(dt);
    m_systems.update<ControlSystem>(dt);
    m_systems.update<CollisionSystem>(dt);
    m_systems.update<MovementSystem>(dt);
    m_systems.update<HighscoreSystem>(dt);
    m_systems.update<LifeTimeSystem>(dt);
    m_systems.update<OrbSpawnSystem>(dt);
    m_systems.update<PathSystem>(dt);
    m_systems.update<EmitterSystem>(dt);
    m_systems.update<RunesSystem>(dt);
}
