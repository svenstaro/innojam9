#ifndef SYSTEM_SOUND_HPP
#define SYSTEM_SOUND_HPP

#include "entityx/entityx.h"
#include "events.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#ifdef __EMSCRIPTEN__
#include <SDL/SDL_mixer.h>
#else
#include <SDL_mixer.h>
#endif

class SoundSystem : public entityx::System<SoundSystem>, public entityx::Receiver<SoundSystem> {

  public:
    SoundSystem(ResourceManager *res_manager) : m_res_manager(res_manager) {
    }

    void configure(entityx::EventManager &event_manager) override {
        event_manager.subscribe<NewWave>(*this);
        event_manager.subscribe<LevelChangedEvent>(*this);
        event_manager.subscribe<PlayerBulletCollison>(*this);
    }
    void receive(const PlayerBulletCollison &event) {
        (void)event;
        Mix_Volume(1, 40);
        Mix_PlayChannel(1, m_res_manager->sound("sound3"), 0);
    }

    void receive(const NewWave &event) {
        (void)event;
        Mix_Volume(1, 40);
        Mix_PlayChannel(1, m_res_manager->sound("sound1"), 0);
    }

    void receive(const LevelChangedEvent &event) {
        (void)event;
        Mix_Volume(2, 40);
        Mix_PlayChannel(2, m_res_manager->sound("sound4"), 0);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override {
        (void)es;
        (void)events;
        (void)dt;
    }

  private:
    ResourceManager *m_res_manager;
    Mix_Music *m_current_song;
};
#endif
