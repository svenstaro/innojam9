#ifndef FUNCTIONS_PATHING_HPP
#define FUNCTIONS_PATHING_HPP

#include "utils.hpp"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/optimum_pow.hpp>
#include <glm/gtc/constants.hpp>

#include "components/component_position.hpp"
#include "components/component_moving.hpp"
#include "components/component_path.hpp"
/*
inline auto create_parable(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    float a1h2 = p1[1] * p1[1];
    float a2h2 = p2[1] * p2[1];
    float a3h2 = p3[1] * p3[1];

    float r2_m_r1_d_a2_m_a1 = (p2[0] - p1[0])/(p2[1] - p1[1]);
    float a2h2_m_a1h2_d_a2_m_a1 = (a2h2 - a1h2)/(p2[1] - p1[1]);

    float a = (p3[0] - p2[0] - (p3[1] - p1[1]) * r2_m_r1_d_a2_m_a1);
    a /= a3h2 - p3[1] * a2h2_m_a1h2_d_a2_m_a1 - a1h2 - p1[1] * a2h2_m_a1h2_d_a2_m_a1;

    float b = r2_m_r1_d_a2_m_a1 - a2h2_m_a1h2_d_a2_m_a1 * a;
    float c = p1[0] - a1h2 * a - p1[1] * b;

    // std::cout << "parable: r = " << a << "*a^2 + " << b << "*a + " << c << std::endl;
    return [=](float a_t){
        float radius = a * a_t*a_t + b * a_t + c;
        float angle = a_t;
        return glm::vec2(radius, angle);
    };
}

//NEVER USE THIS WHEN MOVING STRAIGHT THROUGH THE MIDDLE
inline glm::vec2 parable_path(entityx::Entity entity) {
    //auto pos = entity.component<Position>();
    auto moving = entity.component<Moving>();
    auto path = entity.component<Path>();

    float speed = moving->speed();
    float current_lifetime = path->get_current_lifetime();

    float a_t = path->get_direction()[1] * speed * current_lifetime;

    // std::cout << entity.id() << ": " << path->get_direction()[1] << " * " << speed << " * " << current_lifetime << " = " << a_t << std::endl;
    return path->get_parable()(a_t);
}
*/

inline glm::vec2 scythe_path(glm::vec2 origin, glm::vec2 velocity, float current_lifetime)
{
    glm::vec2 origin_cathesian = polar_to_cathesian(origin);
    glm::vec2 next_position(velocity.x * current_lifetime, sin(current_lifetime) + 45);
    next_position = polar_to_cathesian(next_position);
    return cathesian_to_polar(origin_cathesian + next_position);
}

inline glm::vec2 spiral_path(glm::vec2 origin, glm::vec2 velocity, float current_lifetime)
{
    float speed = velocity.x;
    glm::vec2 origin_cathesian = polar_to_cathesian(origin);
    glm::vec2 next_position(speed * current_lifetime, 10 * sin(current_lifetime));

    next_position = polar_to_cathesian(next_position);

    return cathesian_to_polar(origin_cathesian + next_position);
}

inline glm::vec2 sin_path(glm::vec2 origin, glm::vec2 velocity, float current_lifetime)
{
    float speed = velocity.x;
    glm::vec2 origin_cathesian = polar_to_cathesian(origin);
    glm::vec2 next_position(speed * current_lifetime, sin(current_lifetime * 4) / (current_lifetime * 4));

    next_position = polar_to_cathesian(next_position);

    next_position = cathesian_to_polar(origin_cathesian + next_position);

    next_position.y += velocity.y;
    return next_position;
}


inline glm::vec2 linear_path(glm::vec2 origin, glm::vec2 velocity, float current_lifetime)
{
    glm::vec2 origin_cathesian = polar_to_cathesian(origin);
    origin_cathesian += glm::normalize(polar_to_cathesian(velocity)) * current_lifetime * velocity.x;
    return glm::vec2(cathesian_to_polar(origin_cathesian));
}
#endif
