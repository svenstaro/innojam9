#ifndef MOVEMENT_SYSTEM_CPP
#define MOVEMENT_SYSTEM_CPP

#include "events.hpp"
#include "game.hpp"
#include "component_position.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/gtc/constants.hpp>
#include <type_traits>

class MovementSystem : public entityx::System<MovementSystem>,
                       public entityx::Receiver<MovementSystem> {
  public:
    MovementSystem(float min_dist = 50, float max_dist = 400)
        : m_min_dist(min_dist), m_max_dist(max_dist){
    }

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<PlayerInstructionEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
    }

    void receive(const PlayerInstructionEvent &player_instruction_event) {
        auto copy = player_instruction_event;
        auto position = copy.m_entity.component<Position>();
        auto new_position = position->position() + player_instruction_event.m_direction;

        if(new_position[0] < m_min_dist){
            new_position[0] = m_min_dist;
        }
        else if(new_position[0] > m_max_dist){
            new_position[0] = m_max_dist;
        }
        new_position[1] = glm::mod(2*glm::pi<float>() + new_position[1], 2*glm::pi<float>());
        position->set_position(new_position);
    }

  private:
    float m_min_dist;
    float m_max_dist;
};
#endif
