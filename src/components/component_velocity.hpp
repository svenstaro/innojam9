#ifndef VELOCITY_HPP
#define VELOCITY_HPP

#include "game_config.hpp"
#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct Velocity :entityx::Component<Velocity> {

public:

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

private:

    void accelerating(void)
    {
      m_accelerating = false; // because it will be set on user events
      m_was_accelerating = true;
      m_alpha += m_alpha_step;
      m_full_speed = m_alpha >= 1.0f;
      m_alpha = m_full_speed ? 1.0f : m_alpha;
      m_velocity = glm::mix(m_start_velocity, m_desired_velocity, m_alpha);
    }
    
    void was_accelerating(void)
    {
      m_was_accelerating = false;
      m_alpha = 0.f;
      m_alpha_step = DECELERATION_STEP;
      m_start_velocity = m_velocity;
      m_desired_velocity.x = 0.f;
      m_desired_velocity.y = 0.f;
      m_full_speed = false;
    }
    
    void decelerating(void)
    {
      m_alpha += m_alpha_step;
      m_no_speed = m_alpha >= 1.0f;
      m_alpha = m_no_speed ? 1.0f : m_alpha;
      m_velocity = glm::mix(m_start_velocity, m_desired_velocity, m_alpha);
    }
    
public:
    
    void handle_tick(void)
    {
      if(m_accelerating)
        accelerating();
      else
      {
        if(m_was_accelerating)
          was_accelerating();
        decelerating();
      }
    }
};

#endif
