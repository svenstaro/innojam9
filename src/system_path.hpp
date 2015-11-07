#ifndef SYSTEM_PATH_HPP
#define SYSTEM_PATH_HPP

#include "utils.hpp"
#include "entityx/entityx.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"

#include "component_path.hpp"

class PathSystem : public entityx::System<PathSystem>
{
    public:
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
    {
        entityx::ComponentHandle<Path> path;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Moving> moving;

        glm::vec2 next_step;
        glm::vec2 new_position;

        for(entityx::Entity entity :  es.entities_with_components(path,position,moving))
        {
            if(path->get_time_left() > 0)
            {
                position->set_position(path->m_path_function(entity));

                path->update_current_lifetime(dt);
            }
            else
            {
                entity.destroy();
            }


        }
    }
};

#endif
