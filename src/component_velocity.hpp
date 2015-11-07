#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct Velocity :entityx::Component<Velocity> {
    Velocity()
        : m_velocity({0.0f,0.0f}) {
    }
    
    glm::vec2 m_velocity;
    
    /**
     * Assuming that you can move with the same speed in both directions (radius/angle)
     * this defines the 'max' value for glm::mix.
     * Is reset when acceleration is applied
     */
    glm::vec2 m_desired_velocity;
    
    /**
     * This variable should be set at the start of an acceleration,
     * so that it can be used as the 'min' value for glm::mix
     */
    glm::vec2 m_start_velocity;
    
    /**
     * Current alpha. used for glm::mix
     */
    float m_alpha;
    
    /**
     * Computed at the start of an acceleration
     */
    float m_alpha_step;
    
    /**
     * Flags used to indicate the state of the Movable.
     */
    bool m_full_speed = false;
    bool m_accelerating = false;
    bool m_was_accelerating = false;
    bool m_no_speed = true;
};

#endif
