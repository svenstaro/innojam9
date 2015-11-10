#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "entityx/entityx.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <functional>

    struct Path : entityx::Component<Path> {
        Path(
                std::function<glm::vec2 (glm::vec2, glm::vec2, float)> pathing_function,
                glm::vec2 origin,
                glm::vec2 velocity,
                float max_lifetime)
            : m_path(pathing_function), m_origin(origin), m_velocity(velocity), 
            m_max_lifetime(max_lifetime), m_current_lifetime(0.0f){}   

        float get_time_left()
        {
            return m_max_lifetime - m_current_lifetime;
        }

        float get_current_lifetime()
        {
            return m_current_lifetime;
        }

        void update(float dt)
        {
            
            m_current_lifetime += dt;
        }

        glm::vec2 get_direction()
        {
            return m_velocity;
        }

        glm::vec2 get_origin()
        {
            return m_origin;
        }

        auto get_path(){
            return m_path;
        }
        
        std::function<glm::vec2 (glm::vec2, glm::vec2, float)> m_path;

        glm::vec2 m_origin;
        glm::vec2 m_velocity;

        float m_max_lifetime;
        float m_current_lifetime;
    };

#endif
