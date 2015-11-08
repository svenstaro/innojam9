#ifndef RUNES_SYS
#define RUNES_SYS

#include "component_position.hpp"
#include "component_rune.hpp"
#include "component_light.hpp"
#include "events.hpp"
#include "game.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include "game_config.hpp"

static const int rune_count = 8;
static const glm::i8vec3 rune_color_off{50, 50, 50};
static const glm::i8vec3 rune_color_on{255, 255, 255};

class RunesSystem : public entityx::System<RunesSystem>,
                    public entityx::Receiver<RunesSystem> {
  public:
    RunesSystem() {
    }

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<OrbCollectedEvent>(*this);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
        if(!init_done) {
            init(es);
            init_done = true;
        }
        if(changed) {
            int runes_enabled = int(glm::round(rune_count * progress));
            std::cout << "CHANGED!!!" << progress << "->" << runes_enabled << std::endl;
            auto ru = entityx::ComponentHandle<Rune>();
            auto drw = entityx::ComponentHandle<Drawable>();
            auto lt = entityx::ComponentHandle<Light>();
            for(entityx::Entity entity : es.entities_with_components(ru, drw, lt)) {
                if(runes_enabled-- > 0) {
                    drw->m_colorize = rune_color_on;
                    if(!lt) {
                        entity.assign<Light>("gradient", 0.30f, glm::vec3{200, 0, 0});
                    }
                }
                else {
                    drw->m_colorize = rune_color_off;
                    if(lt) {
                        entity.remove<Light>();
                    }
                }
            }
            changed = false;
        }
    }

    void receive(const OrbCollectedEvent &evt) {
        set_progress(float(evt.m_collected)/float(evt.m_needed));
    }

    void set_progress(float f) {
        progress = f;
        changed = true;
    }

    void init(entityx::EntityManager &es) {
        float radius = RING_OUTER - 32;
        float step = (glm::pi<float>() * 2.f) / float(rune_count);
        for(int i = 0; i < rune_count; i++) {
            entityx::Entity rune = es.create();
            rune.assign<Rune>(i);
            rune.assign<Drawable>("runes", 64, 64, 1, AnimTemplate(32, 32, 1, i), rune_color_off);
            rune.assign<Position>(glm::vec2(radius, i*step));
        }
    }


    bool init_done = false;
    float progress = 0;
    bool changed = false;
};

#endif
