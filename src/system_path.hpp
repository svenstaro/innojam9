#ifndef SYSTEM_PATH_HPP
#define SYSTEM_PATH_HPP

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
            /*
            (void)entity;
            next_step = p; 

            float radius_a = position->position().x;
            float radius_b = next_step.x;

            float angle_a = position->position().y;
            float angle_b = next_step.y;

            new_position.x = glm::sqrt(glm::pow2((radius_a * cos(angle_a) + radius_b * cos(angle_b))) + glm::pow2(radius_a * sin(angle_a) + radius_b * sin(angle_b)));
            new_position.y = angle_a + angle_b;
*/
            position->set_position(path->m_path_function(position->position(), moving->speed(), path->get_current_lifetime()));

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
