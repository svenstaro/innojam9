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
                glm::vec2 direction(radius, angle);
                direction = float(dt) * entity.component<Moving>()->speed() * glm::normalize(direction);
                events.emit<PlayerInstructionEvent>(direction, entity);
            }
        }
    }
};

#endif
