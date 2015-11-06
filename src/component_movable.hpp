#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "component_position.hpp"

struct Movable : Position {
    Movable(double speed = 1)
        : m_speed(speed) {
    }
	/**
	 * Get the speed in pixel per second
	 */
    double speed() {
        return m_speed;
    }

	/**
	 * Set the speed in pixel per second
	 */
    void set_speed(double speed) {
        m_speed = speed;
    }

  private:
	  double m_speed;
};

#endif
