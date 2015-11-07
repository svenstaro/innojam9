#include "game.hpp"
#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_player.hpp"
#include "component_orb.hpp"

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

#include <iostream>

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : m_game(game) {
        int w, h;
        float zoom = 4.0/5.0;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        int wz = w * zoom;
        int hz = h * zoom;
        m_camera = SDL_Rect{w-wz/2, h-hz/5, wz, hz};
        
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

        //Therefore we need these handlers
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
                    render_entity(entity, dt, true);
            }
          }
        }

        auto player_pos = player_entity.component<Position>();

        // RENDER LIGHT

        // Change to render into light rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), m_lighttex);
        SDL_SetRenderDrawColor(m_game->renderer(), 50, 50, 50, 255);
        SDL_RenderClear(m_game->renderer());

        // Draw lights
        for (entityx::Entity entity : es.entities_with_components(position, light)) {
            render_light(entity);
        }

        SDL_SetTextureBlendMode(m_lighttex, SDL_BLENDMODE_MOD);

        // Render to final window. Everything which must appear on the screen HAS
        // to be on one of the textures used here!
        SDL_SetRenderTarget(rendr, m_render_buffer);

        float rotate_by = -rad_to_deg(player_pos->position().y - glm::half_pi<float>());
        SDL_RenderCopyEx(rendr, m_drawtex, nullptr, nullptr, rotate_by, nullptr, SDL_FLIP_NONE);
        SDL_RenderCopyEx(rendr, m_lighttex, nullptr, nullptr, rotate_by, nullptr, SDL_FLIP_NONE);
        
        render_entity(player_entity, dt, false);
        
        SDL_SetRenderTarget(rendr, nullptr);

        SDL_Rect dst{0, 0, 800, 600};
        SDL_RenderGetViewport(rendr, &dst);
        dst.x = dst.y = 0;
        SDL_RenderCopy(rendr, m_render_buffer, &m_camera, &dst);

        auto player = player_entity.component<Player>();
        auto ppos = player_entity.component<Position>();

        if (m_game->is_debug_mode()) {
            SDL_Color c = {200, 200, 200, 100};
            std::string score = "Score: " + std::to_string(player->score);
            std::string pos = "Pos - Radius: " + std::to_string(ppos->position()[0]) + " Angle: " +
                              std::to_string(ppos->position()[1]);
            int orbs = 0;
            int bullets = 0;
            for (entityx::Entity entity : es.entities_with_components(position)) {
                if (entity.component<Path>())
                    bullets++;
                if (entity.component<Orb>())
                    orbs++;
            }
            auto bulletstr = "Bullets: " + std::to_string(bullets);
            auto orbstr = "Orbs: " + std::to_string(orbs);
            auto orbs_collected = "Orbs collected " + std::to_string(m_game->m_orbs_collected);
            auto fps = "FPS: " + std::to_string(1.0 / dt);
            draw_text(rendr, m_game->res_manager(), score, "font20", 0, 0, c);
            draw_text(rendr, m_game->res_manager(), pos, "font20", 0, 20, c);
            draw_text(rendr, m_game->res_manager(), bulletstr, "font20", 0, 40, c);
            draw_text(rendr, m_game->res_manager(), orbstr, "font20", 0, 60, c);
            draw_text(rendr, m_game->res_manager(), orbs_collected, "font20", 0, 80, c);
            draw_text(rendr, m_game->res_manager(), fps, "font20", 0, 100, c);
        } else {
            auto score = "Score: " + std::to_string((int)player_entity.component<Player>()->score);
            SDL_Color c = {200, 200, 200, 0};
            draw_text(rendr, m_game->res_manager(), score, "font20", 0, 0, c);
        }

        // Draw "GUI"
        // freddi TODO

        SDL_RenderPresent(rendr);
    }

  private:
    inline float rad_to_deg(float f)
    {
      return f / glm::two_pi<float>()*360.0;
    }

    void render_entity(entityx::Entity& e, entityx::TimeDelta dt, bool brotate) {
      auto drawable = e.component<Drawable>();
      auto position = e.component<Position>(); //bad name -> change to.. polarpos?

      auto cpy = position->position();
      if(!brotate)
        cpy.y = glm::half_pi<float>();
      glm::vec2 coord_euclid = polar_to_euclid(cpy);

      // Copy the coordinates to dest
      // and offset them by half the image size
      SDL_Rect dest;
      dest.x = coord_euclid.x - drawable->width()/2 + woff;
      dest.y = coord_euclid.y - drawable->height()/2 + hoff;

      dest.w = drawable->width();
      dest.h = drawable->height();

      SDL_Rect* src;
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

      SDL_Texture* tex = m_game->res_manager().texture(drawable->texture_key());
      float rotate = brotate ? rad_to_deg(position->position().y - glm::half_pi<float>()) : 0.0;
      SDL_RenderCopyEx(m_game->renderer(), tex, src, &dest, rotate, nullptr, SDL_FLIP_NONE);
    }
    
    void render_light(entityx::Entity entity)
    {
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
        dest.x = coord_euclid[0] - width/2 + woff;
        dest.y = coord_euclid[1] - height/2 + hoff;
        dest.w = width;
        dest.h = height;

        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
        SDL_SetTextureColorMod(tex, light->color().r, light->color().g, light->color().b);
        SDL_RenderCopy(m_game->renderer(), tex, nullptr, &dest);
    }
  
    int woff;
    int hoff;
  
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *m_lighttex;
    SDL_Texture *m_drawtex;
    SDL_Texture *m_render_buffer;
};
