#ifndef FUNCTIONS_PATHING_HPP
#define FUNCTIONS_PATHING_HPP

glm::vec2 linear_path(glm::vec2 direction, float speed , float dt)
{
    direction.x = speed *dt;
    return direction;         
}

glm::vec2 sin_path(glm::vec2 direction, float speed)
{
    return glm::vec2(0,0);
}



#endif
