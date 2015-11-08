#include "game.hpp"

#include "main_state.hpp"
#include "mainmenu_state.hpp"
#include "game_over_state.hpp"
#include "win_state.hpp"

#include "game_config.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <iostream>
#include <vector>

Game::~Game() {
    m_res_manager.shutdown();
    TTF_Quit();
    SDL_CloseAudio();
    SDL_DestroyRenderer(m_render);
    SDL_DestroyWindow(m_window);
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    m_window = SDL_CreateWindow("Hello World!", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    m_render =
        SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_render == nullptr) {
        SDL_DestroyWindow(m_window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError()
                  << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return 1;
    }

    m_res_manager.load_texture("gradient", "res/gradient.png", m_render);
    m_res_manager.load_texture("player", "res/player.png", m_render);
    m_res_manager.load_texture("fire", "res/fire.png", m_render);
    m_res_manager.load_texture("orb", "res/orb.png", m_render);
    m_res_manager.load_texture("wood", "res/wood.png", m_render);
    m_res_manager.load_texture("inner_bound", "res/inner_bound.png", m_render);
    m_res_manager.load_texture("outer_bound", "res/outer_bound.png", m_render);
    m_res_manager.load_texture("outer_bound_boss", "res/outer_bound_boss.png", m_render);
    m_res_manager.load_texture("bound", "res/bound.png", m_render);

    m_res_manager.load_music("music1", "res/pocket_destroyer.ogg");
    m_res_manager.load_music("music2", "res/orbital_colossus.ogg");

    m_res_manager.load_sound("sound1", "res/whomp.wav");
    m_res_manager.load_sound("sound2", "res/whoomp.wav");
    m_res_manager.load_sound("sound3", "res/hit.wav");
    m_res_manager.load_sound("sound4", "res/levelup.wav");

    m_res_manager.load_texture("bar", "res/bar.png", m_render);
    m_res_manager.load_texture("magma", "res/magma.png", m_render);
    m_res_manager.load_texture("runes", "res/symbols.png", m_render);

    m_res_manager.load_texture("difficulty_easy", "res/menu/easy.png", m_render);
    m_res_manager.load_texture("difficulty_medium", "res/menu/medium.png", m_render);
    m_res_manager.load_texture("difficulty_hard", "res/menu/hard.png", m_render);
    m_res_manager.load_texture("difficulty_svenstaro", "res/menu/svenstaro.png", m_render);

    m_res_manager.load_texture("menu_red_background", "res/menu/background_red.png", m_render);
    m_res_manager.load_texture("menu_green_background", "res/menu/background_green.png", m_render);
    m_res_manager.load_texture("menu_background", "res/menu/background.png", m_render);
    m_res_manager.load_texture("menu_newgame", "res/menu/new_game.png", m_render);
    m_res_manager.load_texture("menu_highscore", "res/menu/highscore.png", m_render);
    m_res_manager.load_texture("menu_exit", "res/menu/exit.png", m_render);
    m_res_manager.load_texture("menu_back", "res/menu/back.png", m_render);
    m_res_manager.load_texture("menu_title", "res/menu/title.png", m_render);

    m_res_manager.load_texture("menu_game_over", "res/menu/game_over.png", m_render);
    m_res_manager.load_texture("menu_win", "res/menu/win.png", m_render);
    m_res_manager.load_texture("menu_game_over_back", "res/menu/game_over_back.png", m_render);

    m_res_manager.load_font("font20", "res/DejaVuSans.ttf", 20);

    SDL_RenderSetLogicalSize(m_render, WIDTH, HEIGHT);

    entityx::Entity entity = m_ex.entities.create();
    entity.assign<Position>();

    //Setting order of levels
    //m_level_vector = {Level::LEVEL_ONE(), Level::LEVEL_TWO(), Level::LEVEL_THREE(), Level::LEVEL_FOUR(),
     //                 Level::LEVEL_FIVE(), Level::LEVEL_SIX(), Level::LEVEL_SEVEN(), Level::LEVEL_EIGHT()};
    m_level_vector = {Level::LEVEL_ONE(), Level::LEVEL_TWO(), Level::LEVEL_THREE()};

    if(DEBUG) {
        m_difficulty = DEBUGDIF;
        m_states.push({"main", std::make_unique<MainState>(this)});
    } else {
        m_states.push({"main_menu", std::make_unique<MainMenuState>(this)});
    }

    m_states.top().second->init();

    return 0;
}

void Game::game_over(bool win, float score) {
    if(win){
        m_states.push({"win", std::make_unique<WinState>(this, score)});
    }
    else{
        m_states.push({"gameover", std::make_unique<GameOverState>(this, score)});
    }
    m_states.top().second->init();

    m_current_level_index = DEBUG ? DEBUG_START_LEVEL: 0;
    m_last_frame_time = 0;
    m_debug_mode = false;
    m_orbs_collected = 0;
}

void Game::next_level()
{
    rumble_for(0.75f);
    m_remaining_lvl_change = 1.5f;
    m_clear_bullets = true;
    //  HIER LEVEL UP SOUND
    m_current_level_index++;
    if(m_current_level_index == m_level_vector.size())
    {
        //TODO GAMEOVER KRAM -> HERE
        m_current_level_index = 0;
    }
}

void Game::mainloop() {
    int current_time = SDL_GetTicks();
    double dt = (current_time - m_last_frame_time) / 1000.0;
    m_last_frame_time = current_time;

    m_states.top().second->update(dt);
}

SDL_Renderer *Game::renderer() {
    return m_render;
}

SDL_Window *Game::window() {
    return m_window;
}

ResourceManager &Game::res_manager() {
    return m_res_manager;
}

std::stack<std::pair<std::string, std::unique_ptr<State>>> &Game::states() {
    return m_states;
}

bool Game::is_running() {
    return m_running;
}

void Game::toggle_debug_mode(void)
{
  m_debug_mode = !m_debug_mode;
}

bool Game::is_debug_mode(void)
{
  return m_debug_mode;
}

void Game::shutdown() {
    m_running = false;
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#endif
}

void Game::set_difficulty(Difficulty difficulty) {
    m_difficulty = difficulty;
}

Difficulty Game::difficulty() {
    return m_difficulty;
}

void Game::popstate() {
    m_states.pop();
}

/** S in seconds */
void Game::rumble_for(float s)
{
  m_remaining_rumble = s;
}

unsigned int Game::get_current_level_index()
{
    return m_current_level_index;
}

unsigned int Game::get_max_level_index(){
    return m_level_vector.size()-1;
}

Level Game::get_current_level()
{
    return m_level_vector[m_current_level_index];
}

const std::string &Game::statename() const {
    return m_states.top().first;
}

const SDL_Rect &Game::world_size() const {
    return m_world_size;
}
