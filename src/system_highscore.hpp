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
    public:

    HighscoreSystem(Game *game) : m_game(game){
    };

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<CollisionEvent>(*this);
        event_manager.subscribe<LevelChangedEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        entityx::ComponentHandle<Player> player;
        for(entityx::Entity entity : es.entities_with_components(player)) {
          (void)entity;
          player->addScore(pts_per_sec* dt);
        }
    }

    void receive(const CollisionEvent &collision_event) {
        auto copy = collision_event; //maybe a copy is not needed

        auto e1 = copy.m_first.component<Player>();
        auto e2 = copy.m_second.component<Orb>();
        if(e1 && e2) {
          e1->addScore(e2->score());
        }
    }

    void receive(const LevelChangedEvent &level_changed_event)
    {
       m_game->m_orbs_collected = 0; 
    }
    
    private:
    Game *m_game;
    float pts_per_sec = -5.0f; // 10 is really high

};

#endif
