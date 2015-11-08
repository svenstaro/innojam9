#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <iostream>

#include "game.hpp"
#include "path_definition.hpp"

#include "entityx/entityx.h"
#include "events.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
#include "game_config.hpp"
#include "patterns.hpp"
#include "component_enemy.hpp"
#include "component_collidable.hpp"
#include "component_orb.hpp"
#include "component_moving.hpp"
#include "component_path.hpp"

#include <glm/glm.hpp>
#include <vector>

class EmitterSystem : public entityx::System<EmitterSystem> {
  public:
    EmitterSystem(Game *game) : m_game(game) {
        update_current_pattern();
        update_current_pattern_part();
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        m_total_elapsed += dt;
        m_last_spawned += dt;

        if (m_last_spawned > m_current_pattern.m_cooldown) {
            Mix_Volume(1, 20);
            Mix_PlayChannel(1, m_game->res_manager().sound("sound1"), 0);

            m_last_spawned = 0.f;
            for(int i = 0; i < m_current_shots_per_cooldown; i++)
            {
                create_bullet(es, m_current_path_function,i);
            }
            m_current_shot++;
            if(m_current_shot == m_current_number_of_max_shots)
            {
                m_current_pattern_part_index++;
                if(m_current_pattern_part_index == m_current_number_of_parts)
                {
                    m_current_pattern_part_index = 0;
                }
                update_current_pattern_part();
                m_current_shot = 0;
            }
            //See if we have to change the level
            if(m_game->m_orbs_collected >= m_current_pattern.m_orbs_needed)
            {
                m_game->next_level();
                events.emit<LevelChangedEvent>();
                m_current_pattern_part_index = 0;
                m_current_shot = 0;
                update_current_pattern();
                update_current_pattern_part();
            }
        }
    }

  private:
    Game *m_game;

    /**
     * The time elapsed since the last spawned particle
     */
    float m_last_spawned = 0.f;

    /**
     * Total elapsed time. Used in the generator.
     */
    float m_total_elapsed = 0.f;
    unsigned int m_current_pattern_part_index = 0;
    unsigned long m_current_shot;
    unsigned long m_current_pattern_length;

    Pattern m_current_pattern = {1.f,1.f,{PatternPart::SIN_TWO_SHOTS()},10};
    float m_current_rotation_speed;
    float m_current_cooldown; 

    void update_current_pattern()
    {
        m_current_pattern = m_game->get_current_level();
        m_current_cooldown = m_current_pattern.m_cooldown;
        m_current_rotation_speed = m_current_pattern.m_rotation_speed;
        m_current_pattern_length = m_current_pattern.m_pattern_parts.size();
    }

    PatternPart m_current_pattern_part = {{},{},{},{}}; 
    Path_Def m_current_path_function = Path_Def(sin_path); 
    int m_current_shots_per_cooldown;
    unsigned int m_current_pattern_parts_length;
    unsigned int m_current_number_of_max_shots;
    unsigned int m_current_number_of_parts;
    float m_current_offset;

    void update_current_pattern_part()
    {
        m_current_pattern_part = 
            m_current_pattern.m_pattern_parts[m_current_pattern_part_index];
        m_current_path_function = 
            m_current_pattern_part.m_shot_type[m_current_pattern_part_index];
        m_current_shots_per_cooldown = 
            m_current_pattern_part.m_shots_per_cooldown[m_current_pattern_part_index];
        m_current_number_of_max_shots = 
            m_current_pattern_part.m_pattern_parts_lengths[m_current_pattern_part_index];
        m_current_number_of_parts = 
            m_current_pattern_part.m_shots_per_cooldown.size();
        m_current_offset = 
            m_current_pattern_part.m_offset[m_current_pattern_part_index];

    }

    void create_bullet(entityx::EntityManager &es, Path_Def path_definition, int i) {
        entityx::Entity next = es.create();

        switch(path_definition.get_path_type()){
            case PARABLE:
                next.assign<Path>(path_definition.get_parable_function(),
                        path_definition.get_origin(),
                        path_definition.get_direction(),
                        20.f);
                break;
            case NORMAL:
                next.assign<Path>(path_definition.get_path_function(),
                        glm::vec2(0,0),
                        glm::vec2(1, glm::radians(m_total_elapsed * m_current_rotation_speed +
                                (m_current_offset + (360.f / m_current_shots_per_cooldown) * i))),
                        20.f);
                break;
        }
        next.assign<Position>(glm::vec2(0.f, 0.f));
        next.assign<Moving>(100.f);
        next.assign<Enemy>();
        next.assign<Collidable>(10.f);
        next.assign<Light>("gradient", 0.5f, glm::vec3{255, 100, 0});
        next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40));
    }
};

#endif
