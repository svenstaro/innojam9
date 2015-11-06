#ifndef SYSTEM_MOVE_BY_VELOCITY_HPP
#define SYSTEM_MOVE_BY_VELOCITY_HPP

class PathSystem : public entityx::System<PathSystem>
{ 
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
    {
        entity::ComponentHandle<Trajectory> trajectory;
        entity::ComponentHandle<Position> position;
        
        glm::vec2 next_step;
        glm::vec2 new_position;

        for(entity::Entity entity :  es.entities_with_components(trajectory,position))
        {
           next_step = m_path_function(position.m_position. m_position.m_speed); 

            float radius_a = positon.m_position.x;
            float radius_b = next_step.x;

            float angle_a = position.m_position.y;
            float angle_b = next_step.y;
            
            new_position.x = glm::sqrt(glm::pow2((radius_a * cos(angle_a) + radius_b * cos(angle_b)) + glm::pow2(radius_a * sin(angle_a) + radius_b * sin(angle_b))));
            new_position.y = angle_a + angle_b;

            position.m_positon.set_postition(new_position); 

        }
    }
}

#endif
