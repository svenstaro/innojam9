#include "patterns.hpp"
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <functional>
#include "entityx/entityx.h"
#include "component_emitter.hpp"
#include "component_lifetime.hpp"
#include "component_drawable.hpp"
#include "component_collidable.hpp"
#include "component_light.hpp"

LayerCompound::LayerCompound(
    std::vector<std::function<glm::vec2(glm::vec2, glm::vec2, float)>> paths,
    std::vector<std::function<void(entityx::EntityManager &, glm::vec2)>> on_death,
    std::vector<unsigned int> number_of_shots, std::vector<float> offset)
    : m_paths(paths), m_on_death(on_death), m_number_of_shots(number_of_shots), m_offset(offset) {
    m_number_of_paths = paths.size();
}

Stage::Stage(std::vector<LayerCompound> stages, std::vector<unsigned int> repitition_per_stage,
             float cooldown)
    : m_path_layers(stages), m_repititions(repitition_per_stage), m_cooldown(cooldown) {
}
void Stage::next() {
    m_current_repition++;
    if (m_current_repition == m_repititions.size()) {
        m_current_repition = 0;
    }
}

LayerCompound Stage::get_current_repitition() {
    return m_path_layers[m_current_repition];
}

Level::Level(std::vector<Stage> stages, unsigned int orbs_to_next_level, float rotations_speed)
    : m_stages(stages), m_orbs_to_next_level(orbs_to_next_level),
      m_rotation_speed(rotations_speed) {
    m_number_of_stages = stages.size();
}

bool Level::is_at_end() {
    if (m_current_stage == 0) {
        return true;
    }
    return false;
}

bool Stage::is_at_end() {
    if (m_current_repition == 0) {
        return true;
    }
    return false;
}
void Level::next() {
    m_current_stage++;
    if (m_current_stage == m_number_of_stages) {
        m_current_stage = 0;
    }
}

Stage Level::get_current_stage() {
    return m_stages[m_current_stage];
}

void nothing(entityx::EntityManager &es, glm::vec2 origin)
{
    (void) origin;
    (void) es;
}

void emit_four(entityx::EntityManager &es, glm::vec2 origin)
{
    entityx::Entity new_entity = es.create();
    new_entity.assign<Position>(origin);
    new_entity.assign<LifeTime>(nothing,2.f);
    new_entity.assign<Drawable>("magma",20,20,4,AnimTemplate(6, 6, 14,0,40));
    new_entity.assign<Emitter>(Level::LEVEL_EIGHT());
    new_entity.assign<Collidable>(10.f);
    new_entity.assign<Light>("gradient", 0.3f, glm::vec3{255, 100, 0});
}

