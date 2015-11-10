#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <iostream>

#include "game.hpp"

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
#include "component_light.hpp"
#include "component_drawable.hpp"

#include <glm/glm.hpp>
#include <vector>

class EmitterSystem : public entityx::System<EmitterSystem> {
    public:
        EmitterSystem(Game *game) : m_game(game) {
        }
        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            m_total_elapsed += dt;
            m_last_spawned += dt;
            if (m_last_spawned > current_stage.m_cooldown) {
                Mix_Volume(1, 20);
                Mix_PlayChannel(1, m_game->res_manager().sound("sound1"), 0);

                m_last_spawned = 0.f;
            m_total_shots++;

                for(unsigned int i = 0; i < current_compound.m_number_of_paths; i++) {
                    for(unsigned int j = 0; j < current_compound.m_number_of_shots[i]; j++) {
                        create_bullet(es, current_compound.m_paths[i],i,j);
                        std::cout << j << " " << i << std::endl;
                    }
                }
                current_compound.m_number_of_shots_done++;

                if(current_compound.m_number_of_shots_done ==
                        current_stage.m_repititions[current_stage.m_current_repition])
                {
                    current_stage.next();
                    current_compound = current_stage.get_current_repitition();

                    if(current_stage.is_at_end()) {
                        current_level.next();
                        current_stage = current_level.get_current_stage();

                        if(current_level.is_at_end())
                        {
                        }
                    }
                }
            }

            if(m_game->m_orbs_collected == current_level.m_orbs_to_next_level) {
                m_game->next_level();
                events.emit<LevelChangedEvent>(m_game->get_current_level_index());
                current_level = m_game->get_current_level();
                current_stage = current_level.get_current_stage();
                current_compound = current_stage.get_current_repitition();
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

    float m_total_shots = 0;

    Level current_level = m_game->get_current_level();
    Stage current_stage = current_level.get_current_stage();
    LayerCompound current_compound = current_stage.get_current_repitition();

    void create_bullet(entityx::EntityManager &es, std::function< glm::vec2(glm::vec2, glm::vec2, float)> path, unsigned int i, unsigned int j) {
        
        entityx::Entity next = es.create();
        glm::vec2 test = glm::vec2(100.f, 10.f * current_level.m_rotation_speed * m_total_elapsed
                    + glm::radians(current_compound.m_offset[i] 
                        + ((360.f / current_compound.m_number_of_shots[i]) * j)));

        next.assign<Path>(path,
                glm::vec2(0.f,0.f),
                test,
                20.f);
        
        next.assign<Position>(glm::vec2(0.f, 0.f));
        next.assign<Moving>(100.f);
        next.assign<Enemy>();
        next.assign<Collidable>(10.f);
        
        if(m_game->get_current_level_index() == m_game->get_max_level_index())
        {
            next.assign<Light>("gradient", 0.3f, glm::vec3{0, 0, 255});
            next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40), glm::i8vec3(100, 100, 100));
        }
        else
        {
            next.assign<Light>("gradient", 0.3f, glm::vec3{255, 100, 0});
            next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40));
        }
    }
};

#endif
