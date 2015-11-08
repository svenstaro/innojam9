#ifndef RUNECOMP_HPP
#define RUNECOMP_HPP

#include "component_drawable.hpp"
#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>


struct Rune : entityx::Component<Rune> {
    Rune(int number) {
    }
};

#endif
