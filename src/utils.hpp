#ifndef UTILS_HPP
#define UTILS_HPP

#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/string_cast.hpp"
#include <stdio.h>
#include <iostream>



inline glm::vec2 add_polar_coordinates(glm::vec2 v1, glm::vec2 v2)
{
    float x_new = v1.x * glm::cos(v1.y) + v2.x * glm::cos(v2.y);
    float y_new = v1.x * glm::sin(v1.y) + v2.x * glm::sin(v2.y);

    //float x_polar_new = glm::sqrt(glm::pow2(x_new) + glm::pow2(y_new)); 
    float x_polar_new = x_new + y_new;
    float angle_polar_new = v2.y + v1.y;

    return glm::vec2(x_polar_new, angle_polar_new);
}

inline glm::vec2 polar_to_cathesian(glm::vec2 polar)
{
    if(polar.x == 0.f)
    {
        return glm::vec2(0.f,0.f);
    }
        return glm::vec2(glm::cos(polar.y) * polar.x,glm::sin(polar.y) * polar.x);
}
inline glm::vec2 cathesian_to_polar(glm::vec2 cathesian)
{
    float radius;
    float angle;
    if(glm::vec2(0.f,0.f) == cathesian)
    {
        return glm::vec2(0.f,0.f);
    }
    radius = glm::sqrt(glm::pow2(cathesian.x) + glm::pow2(cathesian.y));
    
    angle = atan(cathesian.y / cathesian.x);
    if(cathesian.x < 0)
    {
        angle += glm::pi<float>();
    }
    else if(cathesian.y < 0)
    { 
        angle += 2 * glm::pi<float>();
    }
    return glm::vec2(radius, glm::abs(angle));
}

#endif
