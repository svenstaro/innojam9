#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "entityx/entityx.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"

#include <glm/glm.hpp>

class EmitterSystem : public entityx::System<EmitterSystem>
{
private:
    glm::vec2 next_direction(void)
    {
        float total_rads = m_angle_offset * m_total_elapsed;
        return glm::vec2(glm::cos(total_rads), glm::sin(total_rads));
    }

    /**
     * The function which determines the path of the bullets.
     */
    std::function<glm::vec2(glm::vec2, float, float)> m_path;
    
    /**
     * The time elapsed since the last spawned particle
     */
    double m_last_spawned = 0.0;
    
    /**
     * The cooldown between bullet shots. When values close to the 
     * actual delay between frames are used, irregularities can appear.
     */
    double m_cooldown = 1.0;
    
    /**
     * Total elapsed time. Used in the generator.
     */
    double m_total_elapsed = 0.0;
    
    /**
     * The increase in angle per second
     */
    double m_angle_offset = 0.0;

public:
    EmitterSystem(
        std::function<glm::vec2(glm::vec2, float, float)> p,
        double cd,
        double ao) :
        m_path(p), m_cooldown(cd), m_angle_offset(ao)
    {
    }
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
    {
        m_total_elapsed += dt;
        m_last_spawned += dt;
        if(m_last_spawned > m_cooldown)
        {
          m_last_spawned -= m_cooldown;
          entityx::Entity next = es.create();
          next.assign<Position>(glm::vec2(0.f, 0.f));
          next.assign<Path>(m_path, next_direction(), 20.f);
          next.assign<Moving>(100.f);
          next.assign<Light>("gradient", 0.5f, glm::vec3{255, 100, 0});
          next.assign<Drawable>("gradient", 10 , 10);
        }
    }
};

#endif
