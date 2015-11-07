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

#include <SDL_mixer.h>

#include <iostream>
#include <vector>
#include <cstdlib>

#define RESPAWN_TIME 0.2f

class OrbSpawnSystem : public entityx::System<OrbSpawnSystem>,
                       public entityx::Receiver<OrbSpawnSystem> {
  public:
    OrbSpawnSystem(Game *game, entityx::EntityManager &entities) : m_game(game), m_entities(entities), m_delta(0.f) {
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
            if (m_spawn_position.x < 60.f && m_spawn_direction.x < 0) {
                m_spawn_direction.x *= -1;
            }
            if (m_spawn_position.x > 300.f && m_spawn_direction.x > 0) {
                m_spawn_direction.x *= -1;
            }
        }
        for (auto e : orbs_to_delete) {
            // std::cout << "deleting entity with id " << e.id() << std::endl;
            Mix_PlayChannel(1, m_game->res_manager().sound("sound2"), 0);
            e.destroy();
        }

        entityx::ComponentHandle<Orb> orb;
        for (entityx::Entity entity : es.entities_with_components(orb)) {
            orb->update(dt);
            if (orb->time_remaining() < 0) {
                entity.destroy();
            }
        }

        orbs_to_delete.clear();
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
        orb.assign<Light>("gradient", 0.25f, glm::vec3{100, 255, 120});
        orb.assign<Orb>(v * 5, 6.5f);
    }

    void spawn() {
        spawn_at(m_spawn_position);
        // std::cout << "Spawning orb @" << pos.x << "," << pos.y << std::endl;
    }

    Game *m_game;
    std::vector<entityx::Entity> orbs_to_delete;
    entityx::EntityManager &m_entities;
    entityx::TimeDelta m_delta;
    glm::vec2 m_spawn_position;
    glm::vec2 m_spawn_direction;
};
