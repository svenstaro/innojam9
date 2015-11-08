#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"
#include <iostream>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct Player : entityx::Component<Player> {
    Player(float hp) : score(0), m_max_hp(hp), m_hp(hp) {
    }

    float score ;
    float m_max_hp;
    float m_hp;
    float m_hurt = 0.0f;
    bool m_invincible = false;

    void damage(float f) {
      if(!m_invincible)
      {
        m_hp -= f;
        std::cout << "OUCH! " << m_hp << "/" << m_max_hp << std::endl;
      }
    }

    bool is_dead() {
        return m_hp < 0;
    }

    void addScore(float f)
    {
      score = MAX(score+f, 0.0f);
    }
};

#endif
