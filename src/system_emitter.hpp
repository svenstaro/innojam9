#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <iostream>

#include "game.hpp"
#include "path_definition.hpp"

#include "entityx/entityx.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
#include "game_config.hpp"
#include "patterns.hpp"

#include <glm/glm.hpp>
#include <vector>

class EmitterSystem : public entityx::System<EmitterSystem> {
// <<<<<<< HEAD
// =======
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

            if (m_current_pattern == level_vector[m_current_level].m_pattern_parts.size()) {
                m_current_pattern = 0;
                m_current_level = 0;
                m_current_shot = 0;
            }
        }
    }

// >>>>>>> bfff99fa2d1f8cdb3fa9e109e069fe7ca9572010
  private:
    Game *m_game;

    glm::vec2 next_direction(void) {
        return glm::vec2(1, m_total_elapsed);
    }

// <<<<<<< HEAD
//      * The function which determines the path of the bullets.
//      */
//     std::function<glm::vec2(entityx::Entity)> m_path;
//
// =======
// >>>>>>> bfff99fa2d1f8cdb3fa9e109e069fe7ca9572010
    /**
     * The time elapsed since the last spawned particle
     */
    float m_last_spawned = 0.f;

    /**
     * Total elapsed time. Used in the generator.
     */
// <<<<<<< HEAD
//     double m_total_elapsed = 0.0;
//
//     float m_level_time_elapsed = 0.0;
//
//     /**
//      * The increase in angle per second
//      */
//     // double m_angle_offset = 0.0;
//
//
//     int m_shots_per_cooldown = 1;
//
//     float m_rotation_speed = 0;
//
//     int m_current_level = 0;
//
//     std::function<glm::vec2(float)> m_parable = nullptr;
//     glm::vec2 m_origin;
//     float m_parable_direction;
//
//     public:
//     EmitterSystem(
//             Game *game,
//             std::function<glm::vec2(entityx::Entity)> path_function,
//             double cooldown,
//             double angle_offset) :
//         m_game(game), m_path(path_function), m_cooldown(cooldown)//, m_angle_offset(angle_offset)
//     {
//     }
//
//     EmitterSystem(
//             Game *game,
//             std::function<glm::vec2(float)> parable_function,
//             glm::vec2 origin,
//             float direction,
//             double cooldown,
//             double angle_offset) :
//         m_game(game),
//         m_cooldown(cooldown),
//         m_parable(parable_function),
//         m_origin(origin),
//         m_parable_direction(direction)
//         //, m_angle_offset(angle_offset)
//     {
//     }
//
//     void set_speed(float new_speed)
//     {
//         m_rotation_speed = new_speed;
//     }
//
//     void set_shots_per_cooldown(int new_shots_per_cooldown)
//     {
//         m_shots_per_cooldown = new_shots_per_cooldown;
//     }
//
//     void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
//     {
//         m_total_elapsed += dt;
//         m_last_spawned += dt;
//         m_level_time_elapsed += dt;
//         if(m_level_time_elapsed > 5)
//         {
//             m_level_time_elapsed = 0.0f;
//             m_current_level++;
//             m_rotation_speed += 10;
//             m_shots_per_cooldown = m_current_level;
//         }
//
//         if(m_last_spawned > m_cooldown) {
//             Mix_Volume(1, 20);
//             Mix_PlayChannel(1, m_game->res_manager().sound("sound1"), 0);
//
//             m_last_spawned -= m_cooldown;
//             for(int i = 0; i < m_shots_per_cooldown; i++) {
//                 entityx::Entity next = es.create();
//                 if(!m_parable) {
//                     // std::cout << "random" << std::endl;
//                     next.assign<Path>(m_path, glm::vec2(0,0), glm::vec2(1, glm::radians(m_total_elapsed * m_rotation_speed + (360.f/m_shots_per_cooldown) * i)), 20.f);
//                 }
//                 else{
//                     // std::cout << "parable" << std::endl;
//                     next.assign<Path>(m_parable, m_origin, m_parable_direction, 20.f);
//                 }
//                 next.assign<Position>(glm::vec2(0.f, 0.f));
//                 next.assign<Moving>(1.f);
//                 next.assign<Light>("gradient", 0.5f, glm::vec3{255, 100, 0});
//                 next.assign<Drawable>("magma", 10 , 10, 4, AnimTemplate(6, 6, 14, 0, 40));
//             }
//         }
// =======
    float m_total_elapsed = 0.f;

    std::vector<Pattern> level_vector = {level1};
    unsigned int m_current_level = 0;
    unsigned int m_current_pattern = 0;
    unsigned int m_current_shot = 0;

    float current_rotation_speed = level_vector[m_current_level].m_rotation_speed;
    //Das wird nicht geupdated, oder?
    unsigned int current_shots_per_cooldown =
        level_vector[m_current_level].m_pattern_parts[m_current_pattern].m_shots_per_cooldown[0];

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
                            glm::vec2(1, glm::radians(m_total_elapsed * current_rotation_speed +
                                            (360.f / current_shots_per_cooldown) * i)),
                            20.f);
                break;
        }
        next.assign<Position>(glm::vec2(0.f, 0.f));
        next.assign<Moving>(100.f);
        next.assign<Light>("gradient", 0.5f, glm::vec3{255, 100, 0});
        next.assign<Drawable>("magma", 10, 10, 4, AnimTemplate(6, 6, 14, 0, 40));
    }
// >>>>>>> bfff99fa2d1f8cdb3fa9e109e069fe7ca9572010

};

#endif
