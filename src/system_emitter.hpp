#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <iostream>

#include "main_state.hpp"

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
#include "component_emitter.hpp"
#include "component_lifetime.hpp"

#include <glm/glm.hpp>
#include <vector>

class EmitterSystem : public entityx::System<EmitterSystem> {
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        (void) events;
        entityx::ComponentHandle<Emitter> emitter;
        entityx::ComponentHandle<Position> position;
        for (entityx::Entity entity : es.entities_with_components(emitter, position)) {
            (void) entity;
            emitter->m_total_elapsed += dt;
            emitter->m_last_spawned += dt;
//TODO: seperate level and emitter and make it so that each level a new "main" 
// emitter is spawned and at the end of a level deleted
//TODO:
            Stage current_stage = emitter->m_current_stage;
            LayerCompound current_compound = emitter->m_current_compound;

            if (emitter->m_last_spawned > current_stage.m_cooldown) {
                events.emit<NewWave>();

                emitter->m_last_spawned = 0.f;

                for (unsigned int i = 0; i < current_compound.m_number_of_paths; i++) {
                    for (unsigned int j = 0; j < current_compound.m_number_of_shots[i]; j++) {
                        create_bullet(es, current_compound.m_paths[i], i, j, emitter, position);
                    }
                }
                current_compound.m_number_of_shots_done++;

                if (current_compound.m_number_of_shots_done ==
                    current_stage.m_repititions[current_stage.m_current_repition]) {
                    current_stage.next();
                    current_compound = current_stage.get_current_repitition();

                    if (current_stage.is_at_end()) {
                        m_level->next();
                        current_stage = m_level->get_current_stage();
                    }
                }
            }

        }
    }

  private:
    Level *m_level;
    void create_bullet(entityx::EntityManager &es,
                       std::function<glm::vec2(glm::vec2, glm::vec2, float)> path, unsigned int i,
                       unsigned int j, entityx::ComponentHandle<Emitter> emitter,
                       entityx::ComponentHandle<Position> position) {

        entityx::Entity next = es.create();
        glm::vec2 test = glm::vec2(
            100.f,
            10.f * emitter->m_current_level.m_rotation_speed * emitter->m_total_elapsed +
                glm::radians(emitter->m_current_compound.m_offset[i] +
                             ((360.f / emitter->m_current_compound.m_number_of_shots[i]) * j)));

        next.assign<Path>(path, position->position(), test, 20.f);
        next.assign<Position>(position->position());
        next.assign<Moving>(100.f);
        next.assign<Enemy>();
        next.assign<Collidable>(10.f);
        next.assign<LifeTime>(20.f);
            next.assign<Light>("gradient", 0.3f, glm::vec3{255, 100, 0});
            next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40));
/*
        if (Level == m_main_state->get_max_level_index()) {
            next.assign<Light>("gradient", 0.3f, glm::vec3{0, 0, 255});
            next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40),
                                  glm::i8vec3(100, 100, 100));
        } else {
            next.assign<Light>("gradient", 0.3f, glm::vec3{255, 100, 0});
            next.assign<Drawable>("magma", 20, 20, 4, AnimTemplate(6, 6, 14, 0, 40));
        }
        */
    }
};

#endif
