#ifndef SYSTEM_PLAYER_HPP
#define SYSTEM_PLAYER_HPP

#include "entityx/entityx.h"
#include "components/component_player.hpp"
#include "states/main_state.hpp"

class PlayerSystem : public entityx::System<PlayerSystem> {
  public:
    PlayerSystem(MainState *main_state) : m_main_state(main_state) {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Player> player;
        (void)events;
        for (entityx::Entity entity : es.entities_with_components(player)) {
            (void) entity;
            if (player->m_invincible > 0.f) {
                player->m_invincible -= dt;
            }
            if (player->is_dead()) {
                m_main_state->game()->game_over(false, m_main_state->get_score());
            }
        }
    }

  private:
    MainState *m_main_state;
};

#endif
