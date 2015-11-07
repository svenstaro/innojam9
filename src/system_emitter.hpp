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
            return glm::vec2(1, m_total_elapsed);
        }

        /**
         * The function which determines the path of the bullets.
         */
    std::function<glm::vec2(entityx::Entity)> m_path;

    /**
     * The time elapsed since the last spawned particle
     */
    double m_last_spawned = 0.0;

    /**
     * The cooldown between bullet shots. When values close to the
     * actual delay between frames are used, irregularities can appear.
     */
    double m_cooldown = 0.2f;

    /**
     * Total elapsed time. Used in the generator.
     */
    double m_total_elapsed = 0.0;

    float m_level_time_elapsed = 0.0;

    /**
     * The increase in angle per second
     */
    //double m_angle_offset = 0.0;

    int m_shots_per_cooldown = 1;

    float m_rotation_speed = 0;

    int m_current_level = 0;

    public:
    EmitterSystem(
            std::function<glm::vec2(entityx::Entity)> p,
            double cd,
            double ao) :
        m_path(p), m_cooldown(cd)//, m_angle_offset(ao)
    {
    }

    void set_speed(float new_speed)
    {
        m_rotation_speed = new_speed;
    }

    void set_shots_per_cooldown(int new_shots_per_cooldown)
    {
        m_shots_per_cooldown = new_shots_per_cooldown;
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
    {
        m_total_elapsed += dt;
        m_last_spawned += dt;
        m_level_time_elapsed += dt;
        if(m_level_time_elapsed > 5)
        {
            m_level_time_elapsed = 0.0f;
            m_current_level++;
            m_rotation_speed += 10;
            m_shots_per_cooldown = m_current_level;
        }

        if(m_last_spawned > m_cooldown)
        {
            m_last_spawned -= m_cooldown;
            for(int i = 0; i < m_shots_per_cooldown; i++)
            {
                entityx::Entity next = es.create();
                next.assign<Path>(m_path, glm::vec2(1, glm::radians(m_total_elapsed * m_rotation_speed + (360.f/m_shots_per_cooldown) * i)), 20.f);
                next.assign<Position>(glm::vec2(0.f, 0.f));
                next.assign<Moving>(100.f);
                next.assign<Light>("gradient", 0.2f, glm::vec3{255, 100, 0});
                next.assign<Drawable>("magma", 10 , 10, 4, AnimTemplate(6, 6, 14, 0, 40));
            }
        }
    }
};

#endif
