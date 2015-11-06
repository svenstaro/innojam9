#ifndef COLLIDABLE_HPP
#define COLLIDABLE_HPP

#include "entityx/entityx.h"

struct Collidable : entityx::Component<Collidable> {
    Collidable(float radius = 1) {
    }

    float radius() {
        return m_radius;
    }

    void set_radius(float radius) {
        m_radius = radius;
    }

  private:
    float m_radius;
};

#endif
