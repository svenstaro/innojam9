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

class EmitterSystem : public entityx::System<EmitterSystem>
{
    public:
        EmitterSystem(Game *game) : m_game(game) {
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
        {
            m_total_elapsed += dt;
            m_last_spawned += dt;
            if (m_last_spawned > current_stage.m_cooldown) {
                Mix_Volume(1, 20);
                Mix_PlayChannel(1, m_game->res_manager().sound("sound1"), 0);

                m_last_spawned = 0.f;


                for(unsigned int i = 0; i < current_compound.m_number_of_paths; i++)
                {
                    std::cout<< "nop: " <<current_compound.m_number_of_paths << std::endl;
                    for(unsigned int j = 0; j < current_compound.m_number_of_shots[i]; j++)
                    {
                        std::cout << "  nos: " << i <<" - "<< current_compound.m_number_of_shots[i] << std::endl;
                        create_bullet(es, current_compound.m_paths[i],j,i);
                    }
                }
                current_stage.next();
                if(current_stage.is_at_end())
                {
                    current_level.next();
                    current_stage = current_level.get_current_stage();
                    if(current_level.is_at_end())
                    {
                    }
                }
            }
            if(m_game->m_orbs_collected == current_level.m_orbs_to_next_level)
            {
                m_game->next_level();
                events.emit<LevelChangedEvent>();
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

    Level current_level = m_game->get_current_level();
    Stage current_stage = current_level.get_current_stage();
    LayerCompound current_compound = current_stage.get_current_repitition();

    void create_bullet(entityx::EntityManager &es, Path_Def path_definition, unsigned int i, unsigned int j) {
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
                        glm::vec2(1, glm::radians(m_total_elapsed * 1 +
                                (current_compound.m_offset[j] + (360.f / current_compound.m_number_of_shots[j]) * i))),
                        20.f);
                break;
        }
        next.assign<Position>(glm::vec2(0.f, 0.f));
        next.assign<Moving>(100.f);
        next.assign<Enemy>();
        next.assign<Collidable>(10.f);
        next.assign<Light>("gradient", 0.3f, glm::vec3{255, 100, 0});
        next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40));
    }
};

#endif
