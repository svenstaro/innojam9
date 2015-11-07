#include "entityx/entityx.h"
#include "game.hpp"
#include "events.hpp"
#include "component_orb.hpp"
#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_player.hpp"
#include "component_collectable.hpp"
#include "component_collidable.hpp"
#include "component_light.hpp"
#include "anim_template.hpp"

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

#include <iostream>
#include <vector>
#include <cstdlib>

#define RESPAWN_TIME 0.2f

class OrbSpawnSystem : public entityx::System<OrbSpawnSystem>,
                       public entityx::Receiver<OrbSpawnSystem> {
  public:
    OrbSpawnSystem(Game *game, entityx::EntityManager &entities, float min_dist, float max_dist)
      : m_min_dist(min_dist), m_max_dist(max_dist),
        m_game(game), m_entities(entities), m_delta(0.f) {

        m_spawn_direction = glm::vec2((float(std::rand()) / float(RAND_MAX)) * 2.f - 1.f,
                                      (float(std::rand()) / float(RAND_MAX)) * 0.1f - 0.05f);
        m_spawn_position = 100.f * m_spawn_direction;
        m_spawn_position.x += 100.f;
    }

    ~OrbSpawnSystem() {
    }

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<CollisionEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        m_delta += dt;
        if (m_delta >= RESPAWN_TIME) {
            m_delta -= RESPAWN_TIME;
            spawn();
            m_spawn_position += 4.f * m_spawn_direction;
            m_spawn_direction.x += (float(std::rand()) / float(RAND_MAX)) * 4.f - 2.f;
            m_spawn_direction.y +=
                ((float(std::rand()) / float(RAND_MAX)) * 0.2f - 0.1f) / m_spawn_position.x;
            if (m_spawn_position.x < m_min_dist && m_spawn_direction.x < 0) {
                m_spawn_direction.x *= -1;
            }
            if (m_spawn_position.x > m_max_dist && m_spawn_direction.x > 0) {
                m_spawn_direction.x *= -1;
            }
        }
        std::vector<entityx::Entity> new_vec;
        for (auto e : orbs_to_delete) {
            auto o = e.component<Orb>();
            if(o) {
                e.remove<Collidable>();
                e.remove<Light>();
                e.remove<Orb>();
                e.remove<Collectable>();
                Mix_PlayChannel(1, m_game->res_manager().sound("sound2"), 0);
                m_game->m_orbs_collected += 1;
            }
            auto pos = e.component<Position>();
            auto np = pos->position();
            np.x -= 80.f / glm::sqrt(np.x);
            pos->set_position(np);
            if(pos->position().x < 30) {
                e.destroy();
            }
            else {
                new_vec.push_back(e);
            }
        }
        orbs_to_delete.clear();
        orbs_to_delete.insert(orbs_to_delete.end(), new_vec.begin(), new_vec.end());

        entityx::ComponentHandle<Orb> orb;
        for (entityx::Entity entity : es.entities_with_components(orb)) {
            orb->update(dt);
            if (orb->time_remaining() < 0) {
                entity.destroy();
            }
        }
    }

    void receive(const CollisionEvent &event) {
        auto copy = event;
        auto e = copy.m_first.component<Orb>();
        auto p = copy.m_second.component<Player>();
        if (e && p) {
            orbs_to_delete.push_back(copy.m_first);
        }
    }

  private:
    void spawn_at(glm::vec2 &pos) {
        int v = int((float(std::rand()) / float(RAND_MAX)) * 3.f);
        AnimTemplate anim(12, 12, 6, 0, 10);
        entityx::Entity orb = m_entities.create();
        orb.assign<Position>(pos);
        orb.assign<Drawable>("orb", 16, 16, 8, anim);
        orb.assign<Collectable>();
        orb.assign<Collidable>(10);
        orb.assign<Light>("gradient", 0.15f, glm::vec3{0, 0, 255});
        orb.assign<Orb>(v * 5, DECAY_ORBS); //  DECAY_ORBS from game_config.hpp
    }

    void spawn() {
        spawn_at(m_spawn_position);
        // std::cout << "Spawning orb @" << pos.x << "," << pos.y << std::endl;
    }

    float m_min_dist;
    float m_max_dist;

    Game *m_game;
    std::vector<entityx::Entity> orbs_to_delete;
    entityx::EntityManager &m_entities;
    entityx::TimeDelta m_delta;
    glm::vec2 m_spawn_position;
    glm::vec2 m_spawn_direction;
};
