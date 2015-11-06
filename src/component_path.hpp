#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "entityx/entityx.h"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

    struct Path : entityx::Component<Path> { 
        Path(std::function<glm::vec2(glm::vec2, float, float)> path_function, glm::vec2 direction, float max_lifetime)
        : m_path_function(path_function), m_direction(direction),
          m_max_lifetime(max_lifetime), m_current_life_time(0.0f){
          }
    
        std::function<glm::vec2(glm::vec2, float, float)> m_path_function;
        glm::vec2 m_direction;
        float m_max_lifetime;
        float m_current_life_time;
    };

#endif
