#ifndef MAIN_STATE_MAIN_HPP
#define MAIN_STATE_MAIN_HPP

#include "game.hpp"

#include "strapon/state/state.hpp"

#include "entityx/entityx.h"

class MainState : public State, public entityx::Receiver<MainState> {
  public:
    MainState(Game *game);
    ~MainState();
    int init() override;
    void update(double dt) override;
    void update_level();
    Level get_current_level();
    unsigned int get_current_level_index();
    unsigned int get_max_level_index();
    void load_level(unsigned int level_index);
    void clear_level();
    void rumble_for(float lol);
    void update_orb_count();
  private:
    Game *m_game;
    entityx::EventManager m_events;
    entityx::EntityManager m_entities{m_events};
    entityx::SystemManager m_systems{m_entities, m_events};
    std::vector<Level> m_level_vector;
    unsigned int m_current_level_index;
    unsigned int m_number_of_collected_orbs;
    unsigned int m_number_of_needed_orbs;
};

#endif
