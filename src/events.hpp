#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <glm/vec2.hpp>

#include "entityx/entityx.h"

struct PlayerInstructionEvent {
    PlayerInstructionEvent(glm::vec2 velocity, entityx::Entity entity)
        : m_velocity(velocity), m_entity(entity) {
    }

    glm::vec2 m_velocity;
    entityx::Entity m_entity;
};

struct CollisionEvent {
    CollisionEvent(entityx::Entity first, entityx::Entity second)
        : m_first(first), m_second(second) {
    }

    entityx::Entity m_first;
    entityx::Entity m_second;
};

struct OrbCollectedEvent {
    OrbCollectedEvent(unsigned int collected, unsigned int needed)
        : m_collected(collected), m_needed(needed) {
    }

    unsigned int m_collected;
    unsigned int m_needed;
};

struct HitEvent {
    HitEvent() {
    }
};

struct GameOverEvent {
    GameOverEvent(float score) : m_score(score) {
    }
    float m_score;
};

struct LevelChangedEvent
{
    LevelChangedEvent()
    {}
}
;

#endif
