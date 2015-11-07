#include "entityx/entityx.h"
#include "game.hpp"
#include "events.hpp"
#include "component_orb.hpp"
#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_player.hpp"
#include "component_collectable.hpp"
#include "component_collidable.hpp"
#include "component_light.hpp"
#include "anim_template.hpp"
#include "game_config.hpp"

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

#include <iostream>
#include <vector>
#include <cstdlib>

#include <glm/gtc/random.hpp>

class OrbSpawnSystem : public entityx::System<OrbSpawnSystem>,
                       public entityx::Receiver<OrbSpawnSystem> {
  public:
    OrbSpawnSystem(Game *game, entityx::EntityManager &entities, float min_dist, float max_dist)
        : m_min_dist(min_dist), m_max_dist(max_dist), m_game(game), m_entities(entities) {
    }

    ~OrbSpawnSystem() {
    }

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<CollisionEvent>(*this);

        // Add initial orbs
        for (int i = 0; i < MAX_ORBS; ++i) {
            spawn_next(0.01667f);
        }
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        (void)events;

        // Figure out whether we have to spawn a new orb

        auto count_orbs = 0;
        entityx::ComponentHandle<Orb> orb;
        for (entityx::Entity entity : es.entities_with_components(orb)) {
            (void)entity;
            count_orbs += 1;
        }

        if (count_orbs < MAX_ORBS) {
            spawn_next(dt);
        }

        // Deletion logic below
        std::vector<entityx::Entity> new_vec;
        for (auto e : orbs_to_delete) {
            // If there are orbs to delete, remove them gently with a nice animation
            auto o = e.component<Orb>();
            if (o) {
                e.remove<Collidable>();
                e.remove<Light>();
                e.remove<Orb>();
                e.remove<Collectable>();
                Mix_PlayChannel(1, m_game->res_manager().sound("sound2"), 0);
                m_game->m_orbs_collected += 1;
            }
            auto pos = e.component<Position>();
            auto np = pos->position();
            np.x -= 80.f / glm::sqrt(np.x);
            pos->set_position(np);

            if (pos->position().x < 30) {
                // If they are very close to the fire, actually remove them
                e.destroy();
            } else {
                new_vec.push_back(e);
            }
        }
        orbs_to_delete.clear();
        orbs_to_delete.insert(orbs_to_delete.end(), new_vec.begin(), new_vec.end());
    }

    void receive(const CollisionEvent &event) {
        auto copy = event;
        auto e = copy.m_first.component<Orb>();
        auto p = copy.m_second.component<Player>();
        if (e && p) {
            orbs_to_delete.push_back(copy.m_first);
        }
    }

  private:
    void spawn_next(float dt) {
        // auto radius = (RING_OUTER + RING_INNER) / 2.f;
        auto radius = glm::linearRand(RING_OUTER, RING_INNER + 50.f);
        glm::vec2 new_pos{radius, m_last_pos[1] - 50.f * dt};
        m_last_pos = new_pos;
        AnimTemplate anim(12, 12, 6, 0, 10);
        entityx::Entity orb = m_entities.create();
        orb.assign<Position>(new_pos);
        orb.assign<Drawable>("orb", 16, 16, 8, anim);
        orb.assign<Collectable>();
        orb.assign<Collidable>(10);
        orb.assign<Light>("gradient", 0.30f, glm::vec3{100, 100, 255});
        orb.assign<Orb>();
        //std::cout << "spawning at " << dt << " " << glm::to_string(new_pos) << std::endl;
    }

    float m_min_dist;
    float m_max_dist;

    glm::vec2 m_last_pos{0, -1.f};

    Game *m_game;
    std::vector<entityx::Entity> orbs_to_delete;
    entityx::EntityManager &m_entities;
};
