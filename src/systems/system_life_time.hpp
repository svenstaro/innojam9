#ifndef SYSTEM_LIFE_TIME_HPP
#define SYSTEM_LIFE_TIME_HPP

#include "entityx/Entity.h"
#include "components/component_lifetime.hpp"
#include "components/component_position.hpp"

class LifeTimeSystem : public entityx::System<LifeTimeSystem> {

  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        (void) events;
        entityx::ComponentHandle<LifeTime> life_time;
        entityx::ComponentHandle<Position> position;
        for (entityx::Entity entity : es.entities_with_components(life_time, position))
        {
            life_time->update(dt);
            if(life_time->get_current_time_lived() > life_time->get_life_time())
            {
                life_time->m_action_on_death(es, position->position());
                entity.destroy();
            }
        }
    }
};

#endif
