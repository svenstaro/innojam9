#ifndef HIGHSCORE_SYSTEM_CPP
#define HIGHSCORE_SYSTEM_CPP

#include "events.hpp"
#include "game.hpp"
#include "component_player.hpp"
#include "component_orb.hpp"
#include "component_enemy.hpp"

#include "entityx/entityx.h"

#include <type_traits>
#include <iostream>

#define max_immunity 1.0f;

class HighscoreSystem : public entityx::System<HighscoreSystem>,
                        public entityx::Receiver<HighscoreSystem> {
  public:
    HighscoreSystem(Game *game) : m_game(game){};

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<CollisionEvent>(*this);
        event_manager.subscribe<LevelChangedEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        m_events = &events;
        entityx::ComponentHandle<Player> player;
        for (entityx::Entity entity : es.entities_with_components(player)) {
            (void)entity;
            player->addScore(pts_per_sec * dt);
        }
        player->m_hurt = glm::max(0.0f, immunity) / max_immunity;
        if (immunity > 0.0f) {
            immunity -= dt;
        }
        if (hit) {
            hit = false;
            m_game->rumble_for(0.5f);
            damage_enem.destroy();
            if (immunity <= 0.0f) {
                events.emit<HitEvent>();
                player->damage(1.0f);
                if (player->is_dead()) {
                    events.emit<GameOverEvent>(player->score);
                    m_game->game_over(player->score);
                }
                immunity = max_immunity;
            }
        }
    }

    void receive(const CollisionEvent &collision_event) {
        auto copy = collision_event; // maybe a copy is not needed

        auto e1 = copy.m_first.component<Player>();
        auto e2 = copy.m_second.component<Orb>();
        auto e3 = copy.m_second.component<Enemy>();
        if (e1 && e2) {
            e1->addScore(e2->score());
            m_events->emit<OrbCollectedEvent>(m_game->m_orbs_collected, m_game->get_current_level().m_orbs_to_next_level);
        }
        if (e1 && e3) {
            hit = true;
            damage_enem = copy.m_second;
            Mix_PlayChannel(2, m_game->res_manager().sound("sound3"), 0);
        }
    }

    void receive(const LevelChangedEvent &level_changed_event) {
        (void) level_changed_event;
        m_game->m_orbs_collected = 0;
        Mix_PlayChannel(2, m_game->res_manager().sound("sound4"), 0);
    }

  private:
    Game *m_game;
    float pts_per_sec = -0.2f; // 10 is really high
    bool hit = false;
    float immunity = 0.0f;
    entityx::Entity damage_enem;
    entityx::EventManager *m_events;
};

#endif
