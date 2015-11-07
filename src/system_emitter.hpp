#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "game.hpp"
#include "entityx/entityx.h"
#include "events.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
#include "game_config.hpp"
#include "patterns.hpp"

#include <glm/glm.hpp>
#include <vector>

class EmitterSystem : public entityx::System<EmitterSystem> {

  public:
    EmitterSystem(Game *game) : m_game(game) {
    }
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        m_total_elapsed += dt;
        m_last_spawned += dt;

        if (m_last_spawned > level_vector[m_current_level].m_cooldown) {
            Mix_Volume(1, 20);
            Mix_PlayChannel(1, m_game->res_manager().sound("sound1"), 0);

            m_last_spawned = 0.f;
            for (unsigned int j = 0; j < level_vector[m_current_level]
                                             .m_pattern_parts[m_current_pattern]
                                             .m_shots_per_cooldown.size();
                 j++) {
                for (int i = 0; i < level_vector[m_current_level]
                                        .m_pattern_parts[m_current_pattern]
                                        .m_shots_per_cooldown[j];
                     i++) {
                    create_bullet(es, level_vector[m_current_level]
                                          .m_pattern_parts[m_current_pattern]
                                          .m_shot_type[j],
                                  i);
                }
            }
            m_current_shot++;
            if (m_current_shot ==
                    level_vector[m_current_level].m_pattern_parts_lengths[m_current_pattern]) {
                m_current_pattern++;
                m_current_shot = 0;
            }

            //See if we have to change the level
            if(m_game->m_orbs_collected >= level_vector[m_current_level].m_orbs_needed)
            {
                m_current_level++;
                events.emit<LevelChangedEvent>(); 
            }

            //Checks if current level is beyond last level
            if(m_current_level == level_vector.size())
            {
                //GAME FINISHED !!!
                //DUMMY:
                m_current_level = 0;
            }
        }
    }


  private:
    Game *m_game;

    glm::vec2 next_direction(void) {
        return glm::vec2(1, m_total_elapsed);
    }

    /**
     * The time elapsed since the last spawned particle
     */
    float m_last_spawned = 0.f;

    /**
     * Total elapsed time. Used in the generator.
     */
    float m_total_elapsed = 0.f;


    std::vector<Pattern> level_vector = {level1,level2};
    unsigned int m_current_level = 0;
    unsigned int m_current_pattern = 0;
    unsigned int m_current_shot = 0;

    float current_rotation_speed = level_vector[m_current_level].m_rotation_speed;
    unsigned int current_shots_per_cooldown =
        level_vector[m_current_level].m_pattern_parts[m_current_pattern].m_shots_per_cooldown[0];

    void create_bullet(entityx::EntityManager &es,
            std::function<glm::vec2(glm::vec2, float, float)> path_function, int i) {
        entityx::Entity next = es.create();
        next.assign<Path>(path_function,
                glm::vec2(1, glm::radians(m_total_elapsed * current_rotation_speed +
                        (360.f / current_shots_per_cooldown) * i)),
                20.f);
        next.assign<Position>(glm::vec2(0.f, 0.f));
        next.assign<Moving>(100.f);
        next.assign<Light>("gradient", 0.5f, glm::vec3{255, 100, 0});
        next.assign<Drawable>("magma", 10, 10, 4, AnimTemplate(6, 6, 14, 0, 40));
    }
};

#endif
