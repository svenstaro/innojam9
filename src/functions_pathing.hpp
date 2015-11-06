#ifndef FUNCTIONS_PATHING_HPP
#define FUNCTIONS_PATHING_HPP

#include <glm/glm.hpp>

glm::vec2 linear_path(glm::vec2 direction, float speed , float current_lifetime)
{
    direction.x = speed * current_lifetime;
    direction.y = 0.f;
    return direction;         
}

glm::vec2 scythe_path(glm::vec2 direction, float speed, float current_lifetime)
{
    direction.x = speed * current_lifetime;
    direction.y = sin(current_lifetime) + 45;
    return direction;
}

glm::vec2 sin_path(glm::vec2 direction, float speed, float current_lifetime)
{

}



#endif
