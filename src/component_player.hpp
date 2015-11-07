#ifndef PLAYERCOMP_HPP
#define PLAYERCOMP_HPP

#include "entityx/entityx.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct Player : entityx::Component<Player> {
    Player() {
    }
    
    float score = 0;
    
    void addScore(float f)
    {
      score = MAX(score+f, 0.0f);
    }
};

#endif
