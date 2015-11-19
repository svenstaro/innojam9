#ifndef SYSTEM_PATH_HPP
#define SYSTEM_PATH_HPP

#include "utils.hpp"
#include "entityx/entityx.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"
#include <glm/gtx/string_cast.hpp>

#include "component_path.hpp"
#include "component_position.hpp"
#include "component_moving.hpp"
#include "component_lifetime.hpp"

class PathSystem : public entityx::System<PathSystem> {
  public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        (void)events;
        (void)dt;
        entityx::ComponentHandle<Path> path;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Moving> moving;
        entityx::ComponentHandle<LifeTime> life_time;

        glm::vec2 next_step;
        glm::vec2 new_position;

        for (entityx::Entity entity :
             es.entities_with_components(life_time, path, position, moving)) {
            (void)entity;
            glm::vec2 new_position =
                path->m_path(path->m_origin, path->m_velocity, life_time->get_current_time_lived());
            position->set_position(new_position);
        }
    }
};

#endif
