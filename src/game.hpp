#ifndef GAME_HPP
#define GAME_HPP

#include "game_config.hpp"

#include "patterns.hpp"

#include "component_position.hpp"

#include "strapon/state/state.hpp"
#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include "difficulty.hpp"

#include <SDL2/SDL.h>

#include <stack>
#include <string>
#include <vector>

class Game {
  public:
    ~Game();

    int init();
    void mainloop();
    bool is_running();
    void toggle_debug_mode(void);
    bool is_debug_mode(void);
    void shutdown();
    void popstate();
    void rumble_for(float s);
    unsigned int get_current_level_index();
    int get_max_level_index();
    Level get_current_level();
    void next_level();
    void set_difficulty(Difficulty difficulty);
    Difficulty difficulty();
    void game_over(bool win, float score);

    std::stack<std::pair<std::string, std::unique_ptr<State>>> &states();
    const std::string &statename() const;

    SDL_Renderer *renderer();
    ResourceManager &res_manager();
    SDL_Window *window();
    const SDL_Rect &world_size() const;

    unsigned int m_orbs_collected = 0;
    float m_remaining_rumble = 0.0f;
    float m_remaining_lvl_change = 0.0f;
    bool m_clear_bullets = false;
  private:
    bool m_running = true;
    bool m_debug_mode = false;
    int m_last_frame_time = 0;
    unsigned int m_current_level_index = DEBUG ? DEBUG_START_LEVEL: 0;
    std::vector<Level> m_level_vector;
    SDL_Rect m_world_size = {0, 0, WORLD_WIDTH, WORLD_HEIGHT};

    SDL_Renderer *m_render;
    SDL_Window *m_window;
    std::stack<std::pair<std::string, std::unique_ptr<State>>> m_states;
    entityx::EntityX m_ex;
    Difficulty m_difficulty = SVENSTARO;
    ResourceManager m_res_manager;
};

#endif /* end of include guard: GAME_HPP */
