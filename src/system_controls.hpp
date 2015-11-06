#ifndef SYSTEM_CONTROLS_HPP
#define SYSTEM_CONTROLS_HPP

#include "component_moving.hpp"
#include "events.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

class ControlSystem : public entityx::System<ControlSystem> {
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Moving> moving;

        for (entityx::Entity entity : es.entities_with_components(moving)) {
            float radius = 0.0f;
            float angle = 0.0f;

            const Uint8 *state = SDL_GetKeyboardState(NULL);

            if (state[SDL_SCANCODE_W])
                radius -= 1.0f;
            if (state[SDL_SCANCODE_A])
                angle -= 1.0f;
            if (state[SDL_SCANCODE_S])
                radius += 1.0f;
            if (state[SDL_SCANCODE_D])
                angle += 1.0f;

            if (angle != 0.0f || radius != 0.0f) {
                auto moving = entity.component<Moving>();
                auto position = entity.component<Position>();

                glm::vec2 direction(radius, angle);
                direction = float(dt) * glm::normalize(direction);
                direction[0] *= moving->speed();

                //adjust angle speed to match "radial" speed via arc length
                float arc_speed = 0;
                if(position->position()[0] != 0){ //only adjust if radius != 0
                    arc_speed = moving->speed()/position->position()[0];
                }
                direction[1] *= arc_speed;
                events.emit<PlayerInstructionEvent>(direction, entity);
            }
        }
    }
};

#endif
