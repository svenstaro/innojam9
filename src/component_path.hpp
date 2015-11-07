#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "entityx/entityx.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

    struct Path : entityx::Component<Path> {
        Path(std::function<glm::vec2(entityx::Entity)> path_function, glm::vec2 direction, float max_lifetime)
        : m_path_function(path_function), m_direction(direction),
          m_max_lifetime(max_lifetime), m_current_lifetime(0.0f){
          }

        float get_time_left()
        {
            return m_max_lifetime - m_current_lifetime;
        }

        float get_current_lifetime()
        {
            return m_current_lifetime;
        }

        void update_current_lifetime(float dt)
        {
            m_current_lifetime += dt;
        }

        glm::vec2 get_direction()
        {
            return m_direction;
        }

        std::function<glm::vec2(entityx::Entity)> m_path_function;
        glm::vec2 m_direction;
        float m_max_lifetime;
        float m_current_lifetime;


    };

#endif
