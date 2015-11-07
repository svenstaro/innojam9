#ifndef MOVEMENT_SYSTEM_CPP
#define MOVEMENT_SYSTEM_CPP

#include "events.hpp"
#include "game.hpp"
#include "component_position.hpp"
#include "component_velocity.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>
#include <type_traits>
#include <iostream>

class MovementSystem : public entityx::System<MovementSystem>,
                       public entityx::Receiver<MovementSystem> {
  public:
    MovementSystem(float min_dist = 100, float max_dist = 400)
        : m_min_dist(min_dist), m_max_dist(max_dist) {
    }

    void configure(entityx::EventManager &event_manager) override {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        entityx::ComponentHandle<Velocity> velocity;
        entityx::ComponentHandle<Position> position;
        for (entityx::Entity entity : es.entities_with_components(velocity, position)) {
            (void)entity;
            if(velocity->m_accelerating)
            {
              velocity->m_accelerating = false; // because it will be set on user events
              velocity->m_was_accelerating = true;
              velocity->m_alpha += velocity->m_alpha_step;
              velocity->m_full_speed = velocity->m_alpha >= 1.0f;
              velocity->m_alpha = velocity->m_full_speed ? 1.0f : velocity->m_alpha;
              velocity->m_velocity = glm::mix(
                velocity->m_start_velocity,
                velocity->m_desired_velocity,
                velocity->m_alpha);
            }
            else
            {
                if(velocity->m_was_accelerating)
                {
                    velocity->m_was_accelerating = false;
                    velocity->m_alpha = 0.f;
                    velocity->m_alpha_step = 0.05f;
                    velocity->m_start_velocity = velocity->m_velocity;
                    velocity->m_desired_velocity.x = 0.f;
                    velocity->m_desired_velocity.y = 0.f;
                    velocity->m_full_speed = false;
                }
                
                velocity->m_alpha += velocity->m_alpha_step;
                velocity->m_no_speed = velocity->m_alpha >= 1.0f;
                velocity->m_alpha = velocity->m_no_speed ? 1.0f : velocity->m_alpha;
                velocity->m_velocity = glm::mix(
                    velocity->m_start_velocity,
                    velocity->m_desired_velocity,
                    velocity->m_alpha);
                
            }
            
            auto new_position = position->position() + velocity->m_velocity;
            if (new_position.s < m_min_dist) {
                new_position.s = m_min_dist;
            } else if (new_position.s > m_max_dist) {
                new_position.s = m_max_dist;
            }
            float two_pi = 2 * glm::pi<float>();
            new_position.t = glm::mod(two_pi + new_position.t, two_pi);
            position->set_position(new_position);
        }
    }

  private:
    float m_min_dist;
    float m_max_dist;
};
#endif
