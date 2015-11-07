#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "entityx/entityx.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
#include "game_config.hpp"
#include "patterns.hpp"

#include <glm/glm.hpp>
#include <vector>

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
    std::function<glm::vec2(glm::vec2, float, float)> m_path;

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
    double m_angle_offset = 0.0;

    public:
    EmitterSystem(
            std::function<glm::vec2(glm::vec2, float, float)> p,
            double cd,
            double ao) :
        m_path(p), m_cooldown(cd), m_angle_offset(ao)
    {
    }


    std::vector<Pattern> level_vector = {level1};
    unsigned int m_current_level = 0;
    unsigned int m_current_pattern = 0;
    unsigned int m_current_shot = 0;

    float current_rotation_speed = level_vector[m_current_level].m_rotation_speed;
    unsigned int current_shots_per_cooldown = level_vector[m_current_level].m_pattern_parts[m_current_pattern].m_shots_per_cooldown[0];

    void create_bullet(entityx::EntityManager &es, std::function<glm::vec2(glm::vec2, float, float)> path_function, int i)
    {
        entityx::Entity next = es.create();
        next.assign<Path>(path_function, glm::vec2(1, glm::radians(m_total_elapsed * current_rotation_speed + (360.f/current_shots_per_cooldown) * i)), 20.f);
        next.assign<Position>(glm::vec2(0.f, 0.f));
        next.assign<Moving>(100.f);
        next.assign<Light>("gradient", 0.5f, glm::vec3{255, 100, 0});
        next.assign<Drawable>("gradient", 10 , 10);
    }


    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
    {
        m_total_elapsed += dt;
        m_last_spawned += dt;

        if(m_last_spawned > level_vector[m_current_level].m_cooldown)
        {
            m_last_spawned = 0.f;
            for(unsigned int j = 0; j < level_vector[m_current_level].m_pattern_parts[m_current_pattern].m_shots_per_cooldown.size(); j++)
            {
                for(int i = 0; i < level_vector[m_current_level].m_pattern_parts[m_current_pattern].m_shots_per_cooldown[j]; i++)
                {
                    create_bullet(es, level_vector[m_current_level].m_pattern_parts[m_current_pattern].m_shot_type[j], i);
                }
            }
            m_current_shot++;
            if(m_current_shot == level_vector[m_current_level].m_pattern_parts_lengths[m_current_pattern])
            {
                m_current_pattern++;
                m_current_shot = 0;
            }

            if(m_current_pattern == level_vector[m_current_level].m_pattern_parts.size())
            {
                std::cout << "test.txt" << std::endl;
                m_current_pattern = 0;
                m_current_level = 0;
                m_current_shot = 0;
            }

        }
    }
};

#endif
