#ifndef COLLISION_SYSTEM_CPP
#define COLLISION_SYSTEM_CPP

#include "component_position.hpp"
#include "component_collidable.hpp"
#include "events.hpp"
#include "game.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <SDL2/SDL.h>

class CollisionSystem : public entityx::System<CollisionSystem> {
  public:
    CollisionSystem() {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        entityx::ComponentHandle<Position> position1, position2;
        entityx::ComponentHandle<Collidable> collidable1, collidable2;
        for (entityx::Entity entity1 : es.entities_with_components(position1, collidable1)) {
            for (entityx::Entity entity2 : es.entities_with_components(position2, collidable2)) {
                if (entity1 != entity2) {
                    glm::vec2 p1 = position1->position(),
                        p2 = position2->position();
                    float x = glm::cos(p1.y) * p1.x - glm::cos(p2.y) * p2.x;
                    float y = glm::sin(p1.y) * p1.x - glm::sin(p2.y) * p2.x;
                    glm::vec2 intersect = glm::vec2(x, y);
                    float dist = glm::length(intersect);
                    if(dist <= collidable1->radius() + collidable2->radius()) {
                        events.emit<CollisionEvent>(entity1, entity2);
                    }
                }
            }
        }
    }
};

#endif
