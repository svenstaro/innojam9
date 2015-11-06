#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include <glm/vec2.hpp>

    struct Path: entity::Component<Path> 
        Path(std::function<void>(glm::vec2, float) path_function, glm::vec2 direction,float speed, float max_life_time)
        : m_path_function(path_function), m_direction(glm::normalize(direction)), m_speed(speed), m_max_lifetime(max_lifetime), m_curent_life_time(0.0f))
    {
        std::function<void>(glm::vec2, float) m_path_function;
        float m_speed;
        float m_max_lifetime;
        float m_current_life_time;
    }

#endif
