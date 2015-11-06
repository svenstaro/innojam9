#ifndef FUNCTIONS_PATHING_HPP
#define FUNCTIONS_PATHING_HPP

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

glm::vec2 linear_path(glm::vec2 direction, float speed , float current_lifetime)
{
    direction.x = speed * current_lifetime;
    return direction;         
}

glm::vec2 scythe_path(glm::vec2 direction, float speed, float current_lifetime)
{
    direction.x = speed * current_lifetime;
    direction.y = sin(current_lifetime) + 45;
    return direction;
}

glm::vec2 spiral_path(glm::vec2 direction, float speed, float current_lifetime)
{
    direction.x = speed * current_lifetime;
    direction.y = 10 * sin(current_lifetime) / current_lifetime; 
    return direction;
}

glm::vec2 sin_path(glm::vec2 direction, float speed, float current_lifetime)
{
    return glm::vec2(0.f,0.f);
}


#endif
