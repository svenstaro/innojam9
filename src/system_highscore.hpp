#ifndef HIGHSCORE_SYSTEM_CPP
#define HIGHSCORE_SYSTEM_CPP

#include "events.hpp"
#include "main_state.hpp"
#include "component_player.hpp"
#include "component_orb.hpp"
#include "component_enemy.hpp"
#include "component_rune.hpp"
#include "component_light.hpp"
#include "component_ring.hpp"

#include "entityx/entityx.h"

#include <type_traits>
#include <iostream>

#define max_immunity 1.0f;

class HighscoreSystem : public entityx::System<HighscoreSystem>,
                        public entityx::Receiver<HighscoreSystem> {
  public:
    HighscoreSystem(MainState *main_state) : m_main_state(main_state){};

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<PlayerOrbCollison>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        m_events = &events;
        m_entities = &es;
    }
    //TODO: implement level designer
    /*
    void receive(const BossLevelEvent &evt) {
        Mix_HaltMusic();
        Mix_PlayMusic(m_game->res_manager().music("music2"), -1);
        entityx::ComponentHandle<Drawable> draw;
        entityx::ComponentHandle<Ring> ring;
        entityx::ComponentHandle<Rune> rune;
        for (entityx::Entity ent : m_entities->entities_with_components(ring, draw)) {
            (void)ent;
            draw->m_texture_map_key = "outer_bound_boss";
        }
        for (entityx::Entity ent : m_entities->entities_with_components(rune, draw)) {
            ent.remove<Rune>();
            ent.remove<Light>();
            ent.assign<Light>("gradient", 0.7f, glm::vec3{0, 0, 200});
            draw->m_colorize = glm::i8vec3(0, 0, 0);
        }
        for (entityx::Entity ent : m_entities->entities_with_components(draw)) {
            (void)ent;
            //TODO SOMETING IS FUCKED UP HERE
            draw->m_anim = AnimTemplate(32, 32, 1, 2);
        }
    }i
*/
    void receive(const PlayerOrbCollison &event) {
       m_main_state->update_score(event.m_score);
    }
    // TODO
    /*
    void receive(const GameOverEvent &event) {
        m_game->game_over(event.is_win(), event.get_score());
    }*/

  private:
    MainState *m_main_state;
    float pts_per_sec = -0.2f; // 10 is really high
    entityx::Entity damage_enem;
    entityx::EventManager *m_events;
    entityx::EntityManager *m_entities;
};

#endif
