#ifndef RUNES_SYS
#define RUNES_SYS

#include "states/main_state.hpp"

#include "components/component_position.hpp"
#include "components/component_rune.hpp"
#include "components/component_light.hpp"
#include "events.hpp"
#include "game.hpp"

#include "entityx/entityx.h"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <SDL2/SDL.h>
#include "game_config.hpp"

static const int rune_count = 12;
static const glm::i8vec3 rune_color_off{50, 50, 50};
static const glm::i8vec3 rune_color_on{255, 255, 255};

class RunesSystem : public entityx::System<RunesSystem> {
  public:
    RunesSystem(MainState *main_state) : m_main_state(main_state) {
    }

    void configure(entityx::EventManager &event_manager) override {
    }
//TODO: This has to be refactored!
    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override {
        if (!init_done) {
            init(es);
            init_done = true;
        }
            float progress = float(m_main_state->number_of_collected_orbs()) /
                             float(m_main_state->orbs_to_next_level());
            int runes_enabled = int(glm::ceil(rune_count * progress));
            entityx::ComponentHandle<Rune> rune;
            entityx::ComponentHandle<Drawable> drawable;
            for (entityx::Entity entity : es.entities_with_components(rune, drawable)) {
                auto lt = entity.component<Light>();
                if (runes_enabled > 0) {
                    drawable->m_colorize = rune_color_on;
                    if (!lt) {
                        entity.assign<Light>("gradient", 0.7f, glm::vec3{200, 0, 0});
                    }
                    runes_enabled--;
                } else {
                    drawable->m_colorize = rune_color_off;
                    if (lt) {
                        entity.remove<Light>();
                    }
                }
            }
        }

    void init(entityx::EntityManager &es) {
        float radius = RING_INNER;
        float step = (glm::pi<float>() * 2.f) / float(rune_count);
        for (int i = 0; i < rune_count; i++) {
            entityx::Entity rune = es.create();
            rune.assign<Rune>(i);
            rune.assign<Drawable>("runes", 32, 32, 1, AnimTemplate(32, 32, 1, i), rune_color_off);
            rune.assign<Position>(glm::vec2(radius, i * step));
        }
    }

    bool init_done = false;
    MainState *m_main_state;
};

#endif
