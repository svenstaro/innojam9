#ifndef SYSTEM_LIFE_TIME_HPP
#define SYSTEM_LIFE_TIME_HPP

#include "entityx/Entity.h"
#include "component_lifetime.hpp"

class LifeTimeSystem : public entityx::System<LifeTimeSystem> {

  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        (void) events;
        entityx::ComponentHandle<LifeTime> life_time;
        for (entityx::Entity entity : es.entities_with_components(life_time))
        {
            life_time->update(dt);
            if(life_time->get_current_time_lived() > life_time->get_life_time())
            {
                entity.destroy();
            }
        }
    }
};

#endif
