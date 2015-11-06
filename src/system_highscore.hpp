#ifndef HIGHSCORE_SYSTEM_CPP
#define HIGHSCORE_SYSTEM_CPP

#include "events.hpp"
#include "game.hpp"
#include "component_player.hpp"
#include "component_orb.hpp"

#include "entityx/entityx.h"

#include <type_traits>

class HighscoreSystem : public entityx::System<HighscoreSystem>,
                       public entityx::Receiver<HighscoreSystem> {
  private:
    float pts_per_sec = -10.0f;

  public:

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<CollisionEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        entityx::ComponentHandle<Player> player;
        for(entityx::Entity entity : es.entities_with_components(player)) {
          (void)entity;
          player->score += pts_per_sec;
        }



    }

    void receive(const CollisionEvent &collision_event) {
        auto copy = collision_event; //maybe a copy is not needed

        auto e1 = copy.m_first.component<Player>();
        auto e2 = copy.m_second.component<Orb>();
        if(e1 && e2) {
          e1->score += e2->score();
        }
        else {
          auto e1 = copy.m_first.component<Orb>();
          auto e2 = copy.m_second.component<Player>();
          if(e1 && e2) {
            e2->score += e1->score();
          }
        }
    }


};

#endif
