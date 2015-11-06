#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "entityx/entityx.h"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/optimum_pow.hpp"

class EmitterSystem : public entityx::System<EmitterSystem>
{
private:
    std::function<glm::vec2(glm::vec2, float, float)> path;
    double lastSpawned = 0.0;
    double cooldown = 1.0;
    

public:
    EmitterSystem(std::function<glm::vec2(glm::vec2, float, float)> p) :
        path(p)
    {
    }
    
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
    {
        lastSpawned += dt;
        if(lastSpawned > 1.0)
        {
          lastSpawned -= 1.0;
          entityx::Entity next = es.create();
          next.assign<Position>(glm::vec2(0.f, 0.f));
          next.assign<Path>(path, glm::vec2(1.f,45.f), 20.f);
          next.assign<Moving>(100.f);
          next.assign<Drawable>("gradient", 10 , 10);
        }
    }
};

#endif
