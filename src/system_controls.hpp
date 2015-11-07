#ifndef SYSTEM_CONTROLS_HPP
#define SYSTEM_CONTROLS_HPP

#include "component_velocity.hpp"
#include "component_position.hpp"
#include "component_player.hpp"
#include "events.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include<iostream>

class ControlSystem : public entityx::System<ControlSystem> {
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Velocity> velocity_e;
        entityx::ComponentHandle<Player> player;

        for (entityx::Entity entity : es.entities_with_components(player, velocity_e)) {
            auto position = entity.component<Position>();
            float radius = 0.0f;
            float angle = 0.0f;

            const Uint8 *state = SDL_GetKeyboardState(NULL);

            if (state[SDL_SCANCODE_W])
                radius -= 1.0f;
            if (state[SDL_SCANCODE_A])
                angle += 1.0f;
            if (state[SDL_SCANCODE_S])
                radius += 1.0f;
            if (state[SDL_SCANCODE_D])
                angle -= 1.0f;
            if (state[SDL_SCANCODE_UP])
                radius -= 1.0f;
            if (state[SDL_SCANCODE_LEFT])
                angle += 1.0f;
            if (state[SDL_SCANCODE_DOWN])
                radius += 1.0f;
            if (state[SDL_SCANCODE_RIGHT])
                angle -= 1.0f;

            if (angle != 0.0f || radius != 0.0f) {

                glm::vec2 direction(radius, angle);
                glm::vec2 velocity = float(dt) * glm::normalize(direction);
                velocity *= 200.0;
                
                // adjust angle speed to match "radial" speed via arc length
                float arc_speed = 1.0;
                if (position->position()[0] != 0) { // only adjust if radius != 0 ????
                    arc_speed /= position->position()[0];
                }
                velocity[1] *= arc_speed;
                
                velocity_e->m_accelerating = true;
                glm::vec2 d = velocity_e->m_desired_velocity - velocity;
                if(glm::length(d) > 0.01)
                {
                  velocity_e->m_desired_velocity = velocity;
                  velocity_e->m_start_velocity = velocity_e->m_velocity;
                  velocity_e->m_alpha = 0.0f;
                  velocity_e->m_alpha_step = 0.1;
                }
            }
        }
    }
};

#endif
