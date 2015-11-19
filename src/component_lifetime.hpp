
#ifndef COMPONENT_LIFETIME_HPP
#define COMPONENT_LIFETIME_HPP

#include "entityx/entityx.h"
#include "glm/glm.hpp"
#include "glm/vec2.hpp"

struct LifeTime : entityx::Component<LifeTime> {
    LifeTime(std::function<void(entityx::EntityManager &, glm::vec2)> action_on_death, float life_time)
        : m_action_on_death(action_on_death), m_life_time(life_time), m_current_time_lived(0.0f) {
    }
    std::function<void(entityx::EntityManager &, glm::vec2)> m_action_on_death;

    float m_life_time;
    float m_current_time_lived;

    float get_current_time_lived() {
        return m_current_time_lived;
    }

    float get_life_time() {
        return m_life_time;
    }
    void update(float dt) {
        m_current_time_lived += dt;
    }
};

#endif
