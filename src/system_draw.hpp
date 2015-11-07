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

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w, h};
        m_drawtex =
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

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), m_drawtex);
        SDL_SetRenderDrawColor(m_game->renderer(), 0, 100, 200, 255);
        SDL_RenderClear(m_game->renderer());

        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Player> player;
        entityx::ComponentHandle<Light> light;

        glm::vec2 player_pos;
        for (entityx::Entity entity : es.entities_with_components(player, position)) {
            player_pos = polar_to_euclid(entity.component<Position>()->position());
        }

        std::set<int> layers;
        for (entityx::Entity entity : es.entities_with_components(drawable)) {
            (void)entity;
            layers.insert(drawable->layer());
        }

        for (auto layer : layers) {
            for (entityx::Entity entity : es.entities_with_components(drawable, position)) {
                if (drawable->layer() == layer) {
                    auto coord_polar = entity.component<Position>();
                    SDL_Rect dest;

                    // now follow the player
                    glm::vec2 coord_euclid = polar_to_euclid(coord_polar->position());

                    // Converted position
                    dest.x = coord_euclid[0];
                    dest.y = coord_euclid[1];

                    // Center on entity
                    dest.x -= drawable->width() / 2;
                    dest.y -= drawable->height() / 2;

                    // Translate onto player. In fact will do this later. Too confusing w/o other
                    // entities
                    dest.x += m_camera.w / 2;
                    dest.y += m_camera.h / 2;

                    dest.x -= player_pos.x;
                    dest.y -= player_pos.y;

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

                    SDL_RenderCopyEx(m_game->renderer(),
                                     m_game->res_manager().texture(drawable->texture_key()), src,
                                     &dest, 0, nullptr, SDL_FLIP_NONE);
                }
            }
        }

        // Change to render into light rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), m_lighttex);
        SDL_SetRenderDrawColor(m_game->renderer(), 50, 50, 50, 255);
        SDL_RenderClear(m_game->renderer());

        // Draw lights
        for (entityx::Entity entity : es.entities_with_components(position, light)) {
            auto coord_polar = entity.component<Position>();
            SDL_Rect dest;

            // now follow the player
            glm::vec2 coord_euclid = polar_to_euclid(coord_polar->position());

            // Converted position
            dest.x = coord_euclid[0];
            dest.y = coord_euclid[1];

            auto tex = m_game->res_manager().texture(light->texture_key());
            int w, h;
            SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
            auto width = w * light->scale();
            auto height = h * light->scale();

            // Center on entity
            dest.x -= width / 2;
            dest.y -= height / 2;

            // Translate onto player. In fact will do this later. Too confusing w/o other
            // entities
            dest.x += m_camera.w / 2;
            dest.y += m_camera.h / 2;

            dest.x -= player_pos.x;
            dest.y -= player_pos.y;

            dest.w = width;
            dest.h = height;

            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(tex, light->color().r, light->color().g, light->color().b);
            SDL_RenderCopyEx(m_game->renderer(), tex, nullptr, &dest, 0, nullptr, SDL_FLIP_NONE);
        }

        SDL_SetTextureBlendMode(m_lighttex, SDL_BLENDMODE_MOD);

        // Render to final window
        SDL_SetRenderTarget(m_game->renderer(), nullptr);

        auto score = "Score: " + std::to_string(player->score);
        SDL_Color c = {200, 200, 200, 0};
        draw_text(m_game->renderer(), m_game->res_manager(), score, "font20", 0, 0, c);
        SDL_RenderCopy(m_game->renderer(), m_drawtex, &m_camera, nullptr);
        SDL_RenderCopy(m_game->renderer(), m_lighttex, &m_camera, nullptr);
        SDL_RenderPresent(m_game->renderer());
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *m_lighttex;
    SDL_Texture *m_drawtex;
};
