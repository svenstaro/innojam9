#ifndef ORBCOMP_HPP
#define ORBCOMP_HPP

#include "entityx/entityx.h"

struct Orb : entityx::Component<Orb>{
    Orb(float score = 1) : m_score(score) {
    }

    float score() {
        return m_score;
    }

  private:
    float m_score;
};

#endif
