#ifndef COMPONENT_DRAWABLE_HPP
#define COMPONENT_DRAWABLE_HPP

#include "anim_template.hpp"
#include "entityx/entityx.h"
#include <iostream>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct Drawable : entityx::Component<Drawable> {
    Drawable(std::string key, float new_height, float new_width, int layer = 0,
             AnimTemplate anim = AnimTemplate(0, 0), glm::i8vec3 colorize = {0, 0, 0}, bool pulse = false)
        : m_texture_map_key(key), m_height(new_height), m_width(new_width), m_time(0.0),
          m_layer(layer), m_animation_index(0), m_anim(anim), m_colorize(colorize), m_pulse(pulse) {
    }

    float height() {
        return m_height;
    }

    float width() {
        return m_width;
    }

    int layer() {
        return m_layer;
    }

    void set_hight(float new_height) {
        m_height = new_height;
    }

    void set_width(float new_width) {
        m_width = new_width;
    }

    void set_layer(int layer) {
        m_layer = layer;
    }

    int animation_index() {
        return m_animation_index;
    }

    std::string texture_key() {
        return m_texture_map_key;
    }

    AnimTemplate anim() {
        return m_anim;
    }

    void tick(double time) {
        m_time += time;
        if (m_time >= 1.f / m_anim.fps()) {
            m_time = 0;
            m_animation_index = (m_animation_index + 1) % m_anim.length();
        }
    }

    std::string m_texture_map_key;
    float m_height, m_width;
    double m_time;
    int m_layer;
    int m_animation_index;
    AnimTemplate m_anim;
    glm::i8vec3 m_colorize;
    bool m_pulse;

  private:
};
#endif
