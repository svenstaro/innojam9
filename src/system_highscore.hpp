#ifndef HIGHSCORE_SYSTEM_CPP
#define HIGHSCORE_SYSTEM_CPP

#include "events.hpp"
#include "game.hpp"
#include "component_main_emitter.hpp"
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
    HighscoreSystem(Game *game) : m_game(game){};

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<CollisionEvent>(*this);
        event_manager.subscribe<LevelChangedEvent>(*this);
        event_manager.subscribe<GameOverEvent>(*this);
        event_manager.subscribe<BossLevelEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        m_events = &events;
        m_entities = &es;

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
                if(player->is_dead()) {
                    events.emit<GameOverEvent>(false, player->score);
                    // m_game->game_over(false, player->score);
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
            m_events->emit<OrbCollectedEvent>(m_game->m_orbs_collected + 1, m_game->get_current_level().m_orbs_to_next_level);
        }
        if (e1 && e3) {
            hit = true;
            damage_enem = copy.m_second;
            Mix_PlayChannel(2, m_game->res_manager().sound("sound3"), 0);
        }
    }

    void receive(const BossLevelEvent &evt) {
        Mix_HaltMusic();
        Mix_PlayMusic(m_game->res_manager().music("music2"), -1);
        entityx::ComponentHandle<Drawable> draw;
        entityx::ComponentHandle<Ring> ring;
        entityx::ComponentHandle<Rune> rune;
        entityx::ComponentHandle<MainEmitter> main_emitter;
        for(entityx::Entity ent : m_entities->entities_with_components(ring, draw)) {
            (void)ent;
            draw->m_texture_map_key = "outer_bound_boss";
        }
        for(entityx::Entity ent : m_entities->entities_with_components(rune, draw)) {
            ent.remove<Rune>();
            ent.remove<Light>();
            ent.assign<Light>("gradient", 0.7f, glm::vec3{0, 0, 200});
            draw->m_colorize = glm::i8vec3(0, 0, 0);
        }
        for(entityx::Entity ent : m_entities->entities_with_components(main_emitter, draw)) {
            (void)ent;
            draw->m_anim = AnimTemplate(32, 32, 1, 2);
        }
    }

    void receive(const LevelChangedEvent &level_changed_event) {
        (void) level_changed_event;
        m_game->m_orbs_collected = 0;
        Mix_PlayChannel(2, m_game->res_manager().sound("sound4"), 0);

        if(level_changed_event.level() == 0){//> m_game->get_max_level_index()){
            entityx::ComponentHandle<Player> player;
            for(entityx::Entity entity: m_entities->entities_with_components(player)){
                (void)entity;
            }
            if(player){
                m_events->emit<GameOverEvent>(true, player->score);
            }
        }
    }

    void receive(const GameOverEvent& event) {
        m_game->game_over(event.is_win(), event.get_score());
    }

  private:
    Game *m_game;
    float pts_per_sec = -0.2f; // 10 is really high
    bool hit = false;
    float immunity = 0.0f;
    entityx::Entity damage_enem;
    entityx::EventManager *m_events;
    entityx::EntityManager *m_entities;
};

#endif
