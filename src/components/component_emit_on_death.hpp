#ifndef EMIT_ON_DEATH_HPP
#define EMIT_ON_DEATH_HPP


#include "game.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct EmitOnDeath :  entityx::Component<EmitOnDeath> {
    EmitOnDeath(){}
};

#endif
