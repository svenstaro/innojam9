#ifndef UTILS_HPP
#define UTILS_HPP

#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
glm::vec2 add_polar_coordinates(glm::vec2 v1, glm::vec2 v2)
{
    float x_new = v1.x * glm::cos(v1.y) + v2.x * glm::cos(v2.y);
    float y_new = v1.x * glm::sin(v1.y) + v2.x * glm::sin(v2.y);

    //float x_polar_new = glm::sqrt(glm::pow2(x_new) + glm::pow2(y_new)); 
    float x_polar_new = x_new + y_new;
    float angle_polar_new = v2.y + v1.y;

    return glm::vec2(x_polar_new, angle_polar_new);
}

#endif
