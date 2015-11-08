#include "game.hpp"

#include "main_state.hpp"

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
    m_difficulty = EASY;
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
    m_res_manager.load_texture("bound", "res/bound.png", m_render);
    m_res_manager.load_music("music1", "res/pocket_destroyer.ogg");
    m_res_manager.load_music("music2", "res/orbital_colossus.ogg");
    m_res_manager.load_sound("sound1", "res/whomp.wav");
    m_res_manager.load_sound("sound2", "res/whoomp.wav");
    m_res_manager.load_texture("bar", "res/bar.png", m_render);
    m_res_manager.load_texture("magma", "res/magma.png", m_render);
    m_res_manager.load_font("font20", "res/DejaVuSans.ttf", 20);

    SDL_RenderSetLogicalSize(m_render, WIDTH, HEIGHT);

    entityx::Entity entity = m_ex.entities.create();
    entity.assign<Position>();

    //Setting order of levels
    m_level_vector = {Pattern::level1(),Pattern::level2()};
    m_states.push({"main", std::make_unique<MainState>(this)});
    m_states.top().second->init();

    return 0;
}

void Game::next_level()
{
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

unsigned int Game::get_current_level_index()
{
    return m_current_level_index;
}

Pattern Game::get_current_level()
{
    return m_level_vector[m_current_level_index];
}

const std::string &Game::statename() const {
    return m_states.top().first;
}

const SDL_Rect &Game::world_size() const {
    return m_world_size;
}
