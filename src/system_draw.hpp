#include "game.hpp"
#include "component_drawable.hpp"
#include "component_position.hpp"
#include "component_player.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sstream>

#include "strapon/resource_manager/resource_manager.hpp"
#include "strapon/sdl_helpers/sdl_helpers.hpp"

#include<iostream>

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w, h};
        m_drawtex =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                game->world_size().w, game->world_size().h);
        m_maptex =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                game->world_size().w, game->world_size().h);
        m_lighttex =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                game->world_size().w, game->world_size().h);
    }

    ~DrawSystem() {
        SDL_DestroyTexture(m_lighttex);
        SDL_DestroyTexture(m_drawtex);
    }

    glm::vec2 polar_to_euclid(glm::vec2 i) {
        glm::vec2 cart;
        cart.x = glm::cos(i[1]) * i[0];
        cart.y = glm::sin(i[1]) * i[0];
        return cart;
    }
    
    inline double rad_to_deg(double f)
    {
      return f / (2.0*3.14159265)*360.0; //TODO 2 PI
    }
    
    void render_entity(entityx::Entity& e, int woff, int hoff, entityx::TimeDelta dt) {
      auto drawable = e.component<Drawable>();
      auto position = e.component<Position>(); //bad name -> change to.. polarpos?
      
      glm::vec2 coord_euclid = polar_to_euclid(position->position());

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
      SDL_RenderCopyEx(m_game->renderer(), tex, src, &dest, -rad_to_deg(position->position().y), nullptr, SDL_FLIP_NONE);

    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {
        // so that we have less unreadable text
        SDL_Renderer* rendr = m_game->renderer();
        
        //FIRST render everything which is NOT THE PLAYER to maptex
        SDL_SetRenderTarget(rendr, m_maptex);
        SDL_SetRenderDrawColor(rendr, 0, 100, 200, 255);
        SDL_RenderClear(rendr);
        
        // GET the size of the texture so that we can center all drawables
        // btw the texture is 4 times as large as the viewport
        int woff, hoff;
        SDL_QueryTexture(m_maptex, nullptr, nullptr, &woff, &hoff);
        woff /= 2;
        hoff /= 2;
        
        //Therefore we need these handlers
        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Light> light;

        //and we need to sort all the drawables by layers
        std::set<int> layers;
        for (entityx::Entity entity : es.entities_with_components(drawable)) {
            (void)entity;
            layers.insert(drawable->layer());
        }
        
        entityx::Entity player_entity;
        for (auto layer : layers) {
          for (entityx::Entity entity : es.entities_with_components(drawable, position)) {
            if (drawable->layer() == layer && !entity.component<Player>()) {
              render_entity(entity, woff, hoff, dt);
            }
            else if(entity.component<Player>())
              player_entity = entity;
          }
        }
        
        // change the rendertarget to the pre-backbuffer
        SDL_SetRenderTarget(rendr, m_drawtex);
        SDL_SetRenderDrawColor(rendr, 0, 100, 200, 255);
        
        auto player_pos = player_entity.component<Position>();
        drawable = player_entity.component<Drawable>();
        
        // copy a rotated version of the maptexture.
        SDL_RenderCopyEx(rendr, m_maptex, nullptr, nullptr, rad_to_deg(player_pos->position().y), nullptr, SDL_FLIP_NONE);
        
        // Render Player (pretend he is at alpha=-PI/2)
        glm::vec2 coord_polar = player_pos->position();
        coord_polar.y = 3.14159265 / 2.0;
        glm::vec2 coord_euclid = polar_to_euclid(coord_polar);

        // Copy the coordinates to dest 
        // and offset them by half the image size
        SDL_Rect dest;
        dest.x = coord_euclid.x - drawable->width()/2 + woff;
        dest.y = coord_euclid.y - drawable->height()/2 + hoff;

        dest.w = drawable->width();
        dest.h = drawable->height();

        SDL_Texture* tex = m_game->res_manager().texture(drawable->texture_key());
        SDL_RenderCopy(rendr, tex, nullptr, &dest);
        
        // RENDER LIGHT

        // Change to render into light rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), m_lighttex);
        SDL_SetRenderDrawColor(m_game->renderer(), 50, 50, 50, 255);
        SDL_RenderClear(m_game->renderer());

        // Draw lights
        for (entityx::Entity entity : es.entities_with_components(position, light)) {
            auto coord_polar = entity.component<Position>();
            auto coord_euclid = polar_to_euclid(coord_polar->position());
            SDL_Rect dest;

            auto tex = m_game->res_manager().texture(light->texture_key());
            int w, h;
            SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
            auto width = w * light->scale();
            auto height = h * light->scale();

            // Converted position
            dest.x = coord_euclid[0] - width/2;
            dest.y = coord_euclid[1] - height/2;

            dest.w = width;
            dest.h = height;

            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(tex, light->color().r, light->color().g, light->color().b);
            SDL_RenderCopyEx(m_game->renderer(), tex, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
        }

        SDL_SetTextureBlendMode(m_lighttex, SDL_BLENDMODE_MOD);

        // Render to final window
        SDL_SetRenderTarget(rendr, nullptr);

        auto score = "Score: " + std::to_string(player_entity.component<Player>()->score);
        SDL_Color c = {200, 200, 200, 0};
        draw_text(rendr, m_game->res_manager(), score, "font20", 0, 0, c);
        SDL_RenderCopy(rendr, m_drawtex, &m_camera, nullptr);
        SDL_RenderCopy(rendr, m_lighttex, &m_camera, nullptr);
        SDL_RenderPresent(rendr);
        
        
        // Render to final window
        SDL_SetRenderTarget(rendr, nullptr);
        SDL_RenderCopy(rendr, m_drawtex, &m_camera, nullptr);
        SDL_RenderPresent(rendr);
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *m_lighttex;
    SDL_Texture *m_drawtex;
    SDL_Texture *m_maptex;
};
