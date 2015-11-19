#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "entityx/entityx.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <functional>

struct Path : entityx::Component<Path> {
    Path(std::function<glm::vec2(glm::vec2, glm::vec2, float)> pathing_function, glm::vec2 origin,
         glm::vec2 velocity)
        : m_path(pathing_function), m_origin(origin), m_velocity(velocity)
          {
    }

    glm::vec2 get_direction() {
        return m_velocity;
    }

    glm::vec2 get_origin() {
        return m_origin;
    }

    auto get_path() {
        return m_path;
    }

    std::function<glm::vec2(glm::vec2, glm::vec2, float)> m_path;
    std::function<void()> m_despawn;

    glm::vec2 m_origin;
    glm::vec2 m_velocity;

    float m_max_lifetime;
    float m_current_lifetime;
};

#endif
