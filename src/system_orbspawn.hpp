#include "entityx/entityx.h"
#include "game.hpp"
#include <cstdlib>
#include "component_orb.hpp"
#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_player.hpp"
#include "component_collectable.hpp"
#include "component_collidable.hpp"
#include <iostream>
#include <vector>

#define RESPAWN_TIME 1.f

class OrbSpawnSystem : public entityx::System<OrbSpawnSystem>,
                       public entityx::Receiver<OrbSpawnSystem> {
  public:
    OrbSpawnSystem(entityx::EntityManager *entities) : m_entities(entities), m_delta(0.f){

    }

    ~OrbSpawnSystem() {

    }

      void configure(entityx::EventManager &event_manager) override {
          event_manager.subscribe<CollisionEvent>(*this);
      }


    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
        m_delta += dt;
        if(m_delta >= RESPAWN_TIME) {
            m_delta -= RESPAWN_TIME;
            spawn();
        }
        while(orbs_to_delete.size() > 0) {
            entityx::Entity &e = orbs_to_delete.back();
            //std::cout << "deleting entity with id " << e.id() << std::endl;
            e.destroy();
            orbs_to_delete.pop_back();
        }
    }

    void receive(const CollisionEvent &event) {
        auto copy = event;
        auto e = copy.m_first.component<Orb>();
        auto p = copy.m_second.component<Player>();
        if(e && p) {
            orbs_to_delete.push_back(copy.m_first);
        }
    }

  private:

    void spawn() {
        entityx::Entity orb = m_entities->create();
        glm::vec2 pos = glm::vec2(50.f + 250.f * (float(std::rand()) / float(RAND_MAX)), (float(std::rand()) / float(RAND_MAX)) * glm::pi<float>() * 2.f);
        orb.assign<Position>(pos);
        orb.assign<Drawable>("orb", 20, 20, 8);
        orb.assign<Collectable>();
        orb.assign<Collidable>(10);
        orb.assign<Orb>(10);
        //std::cout << "Spawning orb @" << pos.x << "," << pos.y << std::endl;
    }

    std::vector<entityx::Entity> orbs_to_delete;
    entityx::EntityManager *m_entities;
    entityx::TimeDelta m_delta;
};