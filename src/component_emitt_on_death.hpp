#ifndef EMITT_ON_DEATH_HPP
#define EMITT_ON_DEATH_HPP


#include "game.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct EmittOnDeath :  entityx::Component<EmittOnDeath> {
    EmittOnDeath(){}
};

#endif
