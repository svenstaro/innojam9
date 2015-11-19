#ifndef ORBCOMP_HPP
#define ORBCOMP_HPP

#include "entityx/entityx.h"

struct Orb : entityx::Component<Orb>{
    Orb(unsigned int score = 1, float max_lifetime = 0.f) : m_score(score), m_max_lifetime(max_lifetime), m_current_lifetime(0.f) {
    }

    unsigned int score() {
        return m_score;
    }

    float time_remaining()
    {
        return m_max_lifetime - m_current_lifetime;
    }

    void update(float dt)
    {
        m_current_lifetime += dt; 
    }

  private:
    unsigned int m_score;
    float m_max_lifetime;
    float m_current_lifetime;
};

#endif
