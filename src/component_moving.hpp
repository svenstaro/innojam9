#ifndef MOVING_HPP
#define MOVING_HPP

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include "component_position.hpp"

struct Moving :Position { //everzthing that can be moving also has to have a position
    Moving(float speed = 1)
        : m_speed(speed) {
    }

	float speed() {
		return m_speed;
	}

	void set_speed(float speed) {
		m_speed = speed;
	}

  private:
    float m_speed;
};

#endif
