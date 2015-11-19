#ifndef COMPONENT_EMITTER
#define COMPONENT_EMITTER

#include "entityx/entityx.h"
#include "patterns.hpp"

struct Emitter : entityx::Component<Emitter> {
    Emitter(Level start_level)
        : m_current_level(start_level), m_current_stage(start_level.get_current_stage()),
          m_current_compound(m_current_stage.get_current_repitition()), m_total_elapsed(0.f),
          m_last_spawned(0.f) {
    }

  public:
    Level m_current_level;
    Stage m_current_stage;
    LayerCompound m_current_compound;
    float m_total_elapsed;
    float m_last_spawned;
};
#endif
