#ifndef COMPONENT_DRAWABLE_HPP
#define COMPONENT_DRAWABLE_HPP

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

struct Drawable : entityx::Component<Drawable> {
    Drawable(std::string key, float new_height, float new_width, int layer = 0)
        : m_texture_map_key(key), m_height(new_height), m_width(new_width), m_layer(layer) {
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

    std::string texture_key() {
        return m_texture_map_key;
    }

  private:
    std::string m_texture_map_key;
    float m_height;
    float m_width;
    int m_layer;
};
#endif
