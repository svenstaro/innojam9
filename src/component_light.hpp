#ifndef COMPONENT_LIGHT_HPP
#define COMPONENT_LIGHT_HPP

#include "entityx/entityx.h"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

struct Light : entityx::Component<Light> {
    Light(std::string key, float scale = 1.f, glm::i8vec3 color = {255, 255, 255})
        : m_texture_map_key(key), m_scale(scale), m_color(color) {
    }

    std::string texture_key() {
        return m_texture_map_key;
    }

    float scale() {
        return m_scale;
    }

    void set_scale(float new_scale) {
        m_scale = new_scale;
    }

    glm::i8vec3 color() {
        return m_color;
    }

    void set_color(glm::i8vec3 new_color) {
        m_color = new_color;
    }

  private:
    std::string m_texture_map_key;
    float m_scale;
    glm::i8vec3 m_color;
};
#endif
