#include "events.hpp"
#include "game.hpp"
#include "game_config.hpp"
#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_player.hpp"
#include "component_enemy.hpp"
#include "component_orb.hpp"
#include "component_light.hpp"
#include "component_velocity.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <glm/gtc/constants.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>

#include "strapon/resource_manager/resource_manager.hpp"
#include "strapon/sdl_helpers/sdl_helpers.hpp"

#include <random>

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : m_game(game), distribution(0.0, 2.5) {
        int w, h;
        float zoom = 4.0 / 5.0;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_w = w;
        m_h = h;
        int wz = w * zoom;
        int hz = h * zoom;
        m_camera = SDL_Rect{w - wz / 2, h - hz / 5, wz, hz};

        int game_w = game->world_size().w;
        int game_h = game->world_size().h;

        m_drawtex = SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN,
                                      SDL_TEXTUREACCESS_TARGET, game_w, game_h);

        m_lighttex = SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN,
                                       SDL_TEXTUREACCESS_TARGET, game_w, game_h);

        m_render_buffer = SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN,
                                            SDL_TEXTUREACCESS_TARGET, game_w, game_h);

        woff = game_w / 2;
        hoff = game_h / 2;
    }

    ~DrawSystem() {
        SDL_DestroyTexture(m_lighttex);
        SDL_DestroyTexture(m_drawtex);
        SDL_DestroyTexture(m_render_buffer);
    }

    glm::vec2 polar_to_euclid(glm::vec2 i) {
        glm::vec2 cart;
        cart.x = glm::cos(i[1]) * i[0];
        cart.y = glm::sin(i[1]) * i[0];
        return cart;
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        // so that we have less unreadable text
        SDL_Renderer *rendr = m_game->renderer();

        // FIRST render everything to drawtex
        SDL_SetRenderTarget(rendr, m_drawtex);
        SDL_SetRenderDrawColor(rendr, 0, 100, 200, 255);
        SDL_RenderClear(rendr);

        // Therefore we need these handlers
        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Light> light;

        // and we need to sort all the drawables by layers
        std::set<int> layers;
        for (entityx::Entity entity : es.entities_with_components(drawable)) {
            (void)entity;
            layers.insert(drawable->layer());
        }

        entityx::Entity player_entity;
        for (auto layer : layers) {
            for (entityx::Entity entity : es.entities_with_components(drawable, position)) {
                if (drawable->layer() == layer) {
                    if (entity.component<Player>())
                        player_entity = entity;
                    else
                        render_entity(entity, dt, true, 0.0f);
                }
            }
        }

        entityx::ComponentHandle<Position> player_pos = player_entity.component<Position>();
        entityx::ComponentHandle<Player> player = player_entity.component<Player>();
        entityx::ComponentHandle<Velocity> velocity = player_entity.component<Velocity>();
        (void)velocity;

        // RENDER LIGHT

        // Change to render into light rendertexture for now
        SDL_SetRenderTarget(rendr, m_lighttex);
        SDL_SetRenderDrawColor(rendr, 50, 50, 50, 255);
        SDL_RenderClear(rendr);

        // Draw lights
        for (entityx::Entity entity : es.entities_with_components(position, light)) {
            render_light(entity);
        }

        SDL_SetTextureBlendMode(m_lighttex, SDL_BLENDMODE_MOD);

        // Render to final window. Everything which must appear on the screen HAS
        // to be on one of the textures used here!
        SDL_SetRenderTarget(rendr, m_render_buffer);

        /*float maximum_rotate = MAX_SPEED / RING_INNER;
        float beta = velocity->m_velocity.t / maximum_rotate;
        // std::cout << beta << std::endl;
        float add_rotate = 100.0 * beta * glm::pi<float>()/8;*/

        float rotate_by = -rad_to_deg(player_pos->position().y - glm::half_pi<float>());
        SDL_SetTextureColorMod(m_drawtex, 255 - 64 * player->m_hurt, 255 - 229 * player->m_hurt,
                               255 - 220 * player->m_hurt);
        SDL_RenderCopyEx(rendr, m_drawtex, nullptr, nullptr, rotate_by, nullptr, SDL_FLIP_NONE);
        SDL_RenderCopyEx(rendr, m_lighttex, nullptr, nullptr, rotate_by, nullptr, SDL_FLIP_NONE);

        render_entity(player_entity, dt, false, 0.0f);

        // BLACKOUT ON LEVEL CHANGE (and game over)
        float alpha = m_game->m_remaining_lvl_change > 0.75f
                          ? 1.f - (m_game->m_remaining_lvl_change - 1.f) / 0.5f
                          : m_game->m_remaining_lvl_change / 0.5f;
        alpha = glm::clamp(alpha, 0.0f, 1.0f);
        // TODO MAINSTATE OWN M_REMAINING...
        m_game->m_remaining_lvl_change -= dt;
        // TODO PLAYER SHIT NOT HERE
        //player->m_invincible = alpha >= 0.1;

        SDL_SetTextureColorMod(m_render_buffer, 255 - 255 * alpha, 255 - 255 * alpha,
                               255 - 255 * alpha);

        SDL_SetRenderTarget(rendr, nullptr);

        // GENERATE RUMBLE NOISE
        SDL_Rect dst{0, 0, 800, 600};
        SDL_RenderGetViewport(rendr, &dst);
        dst.x = dst.y = 0;
        dst.x += distribution(generator) * m_game->m_remaining_rumble;
        dst.y += distribution(generator) * m_game->m_remaining_rumble;
        float angle = distribution(generator) * m_game->m_remaining_rumble;
        m_game->m_remaining_rumble = glm::max(0.0f, m_game->m_remaining_rumble - float(dt));

        // FINAL RENDER CALL!
        SDL_RenderCopyEx(rendr, m_render_buffer, &m_camera, &dst, angle, nullptr, SDL_FLIP_NONE);

        int orbs = 0;
        int bullets = 0;
        for (entityx::Entity entity : es.entities_with_components(position)) {
            if (entity.component<Path>())
                bullets++;
            if (entity.component<Orb>())
                orbs++;
        }

        if (m_game->is_debug_mode()) {
            SDL_Color c = {200, 200, 200, 100};
            //TODO std::string score = "Score: " + std::to_string(player->score);
            std::string pos = "Pos - Radius: " + std::to_string(player_pos->position()[0]) +
                              " Angle: " + std::to_string(player_pos->position()[1]);
            auto bulletstr = "Bullets: " + std::to_string(bullets);
            auto orbstr = "Orbs: " + std::to_string(orbs);
            auto orbs_collected = "Orbs collected " + std::to_string(m_game->m_orbs_collected);
            auto fps = "FPS: " + std::to_string(1.0 / dt);
            //draw_text(rendr, m_game->res_manager(), score, "font20", 0, 0, c);
            draw_text(rendr, m_game->res_manager(), pos, "font20", 0, 20, c);
            draw_text(rendr, m_game->res_manager(), bulletstr, "font20", 0, 40, c);
            draw_text(rendr, m_game->res_manager(), orbstr, "font20", 0, 60, c);
            draw_text(rendr, m_game->res_manager(), orbs_collected, "font20", 0, 80, c);
            draw_text(rendr, m_game->res_manager(), fps, "font20", 0, 100, c);
        } else {
//TODO: 
            //auto score = "Score: " + std::to_string((int)player->score);
            //SDL_Color c = {200, 200, 200, 0};
            //draw_text(rendr, m_game->res_manager(), score, "font20", 20, 20, c);
        }
        render_bar(rendr, player->m_hp, player->m_max_hp);

        // TODO: TO DA MAINSTATE!
        // SDL_Color c = {200, 200, 200, 100};
        // auto current_level = "Level " + std::to_string(m_main_state->get_current_level_index() +
        // 1);
        // draw_text(rendr, m_game->res_manager(), current_level, "font20", 600, 20, c);
        SDL_RenderPresent(rendr);
    }

  private:
    void render_bar(SDL_Renderer *rendr, float i, float max) {
        auto bar_tex = m_game->res_manager().texture("bar");
        int w, h;
        int padding = 0;
        int scale = 2;
        int bar_width = m_w - padding * 2;
        SDL_GetWindowSize(m_game->window(), &w, &h);
        SDL_Rect bar_src;
        SDL_Rect bar_dst;
        int fill = int(glm::ceil((i / max) * bar_width));
        for (int i = 0; i < bar_width; i += 64) {
            int width = glm::min(bar_width - i, 64 * scale);
            bar_src = SDL_Rect{32, 16, width / scale, 16};
            bar_dst = SDL_Rect{padding + i, m_h - padding - 16 * scale, width, 16 * scale};
            SDL_RenderCopy(rendr, bar_tex, &bar_src, &bar_dst);
            if (i < fill) {
                int fill_width = glm::min(fill - i, 64 * scale);
                bar_src = SDL_Rect{32, 0, fill_width / scale, 16};
                bar_dst = SDL_Rect{padding + i, m_h - padding - 16 * scale, fill_width, 16 * scale};
                SDL_RenderCopy(rendr, bar_tex, &bar_src, &bar_dst);
            }
        }
    }
    inline float rad_to_deg(float f) {
        return f / glm::two_pi<float>() * 360.0;
    }

    /**
     * Renders an entity.
     * TimeDelta used for animation
     * brotate specifies, whether the entity should be rotated on the world-map, or
     * if it just stays on the bottomline (like the player)
     * if brotate is false, add_rotate is added to all angles angle
     *
     * The polar coords are translated to euclidean, so that we know where to put the object
     * here you cann add the angle to move the object relative to the world
     * Then, the texture is rotated, so that its orientation stays constant
     * relative to the center.
     */
    void render_entity(entityx::Entity &e, entityx::TimeDelta dt, bool brotate, float add_rotate) {
        auto drawable = e.component<Drawable>();
        auto position = e.component<Position>(); // bad name -> change to.. polarpos?

        auto cpy = position->position();
        if (!brotate)
            cpy.y = glm::half_pi<float>() + add_rotate;
        glm::vec2 coord_euclid = polar_to_euclid(cpy);

        // Copy the coordinates to dest
        // and offset them by half the image size
        SDL_Rect dest;
        dest.x = coord_euclid.x - drawable->width() / 2 + woff;
        dest.y = coord_euclid.y - drawable->height() / 2 + hoff;

        dest.w = drawable->width();
        dest.h = drawable->height();

        SDL_Rect *src;
        AnimTemplate anim = drawable->anim();
        if (anim.frame_width() == 0 || anim.frame_height() == 0) {
            src = NULL;
        } else {
            SDL_Rect s;
            s.x = anim.frame_width() * drawable->animation_index();
            s.y = anim.frame_height() * anim.y_index();
            s.w = anim.frame_width();
            s.h = anim.frame_height();
            drawable->tick(dt);
            src = &s;
        }

        SDL_Texture *tex = m_game->res_manager().texture(drawable->texture_key());
        float rotate =
            brotate ? rad_to_deg(position->position().y - glm::half_pi<float>()) : add_rotate;

        if (drawable->m_colorize != glm::i8vec3{0}) {
            SDL_SetTextureColorMod(tex, drawable->m_colorize.r, drawable->m_colorize.g,
                                   drawable->m_colorize.b);
        }

        SDL_RenderCopyEx(m_game->renderer(), tex, src, &dest, rotate, nullptr, SDL_FLIP_NONE);

        if (drawable->m_colorize != glm::i8vec3{0}) {
            SDL_SetTextureColorMod(tex, 0, 0, 0);
        }
    }

    void render_light(entityx::Entity entity) {
        auto coord_polar = entity.component<Position>();
        auto coord_euclid = polar_to_euclid(coord_polar->position());

        entityx::ComponentHandle<Light> light = entity.component<Light>();
        auto tex = m_game->res_manager().texture(light->texture_key());
        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        auto width = w * light->scale();
        auto height = h * light->scale();

        // Converted position
        SDL_Rect dest;
        dest.x = coord_euclid[0] - width / 2 + woff;
        dest.y = coord_euclid[1] - height / 2 + hoff;
        dest.w = width;
        dest.h = height;

        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
        SDL_SetTextureColorMod(tex, light->color().r, light->color().g, light->color().b);
        SDL_RenderCopy(m_game->renderer(), tex, nullptr, &dest);
    }

    int woff;
    int hoff;
    int m_w, m_h;

    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *m_lighttex;
    SDL_Texture *m_drawtex;
    SDL_Texture *m_render_buffer;

    std::default_random_engine generator;
    std::normal_distribution<float> distribution;
};
