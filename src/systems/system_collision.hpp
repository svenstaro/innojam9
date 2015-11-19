#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "components/component_position.hpp"
#include "components/component_collidable.hpp"
#include "components/component_player.hpp"
#include "components/component_orb.hpp"

#include "events.hpp"

#include "states/main_state.hpp"

#include "entityx/entityx.h"

#include "utils.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <SDL2/SDL.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
  public:
    CollisionSystem(MainState *main_state) : m_main_state(main_state) {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        (void)dt;

        entityx::ComponentHandle<Position> position_player, position_other;
        entityx::ComponentHandle<Collidable> collidable_player, collidable_other;

        entityx::ComponentHandle<Player> player;
        entityx::ComponentHandle<Path> path;

        //Here we check for collision betweet the player and a bullet
        for (entityx::Entity player_entity :
             es.entities_with_components(player, position_player, collidable_player)) {
            for (entityx::Entity bullet_entity :
                 es.entities_with_components(position_other, collidable_other, path)) {
                if (check_for_collision(position_player, position_other, collidable_player,
                                        collidable_other)&& player->m_invincible <= 0.f) {

                    events.emit<PlayerBulletCollison>(player_entity, bullet_entity);
                    bullet_entity.destroy();
                    m_main_state->rumble_for(0.75f);
                    player->damage(1.f);
                    player->m_invincible = 1.f;
                }
            }
        }

        //Here we check for collision betweet the player and an orb
        entityx::ComponentHandle<Orb> orb;

        for (entityx::Entity player_entity :
             es.entities_with_components(player, position_player, collidable_player)) {
            (void)player_entity;
            for (entityx::Entity orb_entity :
                 es.entities_with_components(orb, position_other, collidable_other)) {
                if (check_for_collision(position_player, position_other, collidable_player,
                                        collidable_other)) {
                    m_main_state->update_orb_count();
                    events.emit<PlayerOrbCollison>(orb->score(), orb_entity);
                }
            }
        }
    }

  private:
    MainState *m_main_state;

    bool check_for_collision(entityx::ComponentHandle<Position> position_player,
                             entityx::ComponentHandle<Position> position_other,
                             entityx::ComponentHandle<Collidable> collidable_player,
                             entityx::ComponentHandle<Collidable> collidable_other) {
        float dist = glm::length(polar_to_cathesian(
            position_player->position()) - polar_to_cathesian(position_other->position()));

        return dist <= collidable_player->radius() + collidable_other->radius();
    }
};
#endif
