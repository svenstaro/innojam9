#include "menu_state.hpp"

#include "entityx/entityx.h"

#include "component_position.hpp"
#include "component_drawable.hpp"
#include "component_menuitem.hpp"

#include "anim_template.hpp"

#include <glm/vec2.hpp>

#include <SDL.h>
#include <iostream>



MenuState::MenuState(Game *game): m_game(game){

}

MenuState::~MenuState(){
	SDL_DestroyTexture(m_drawtex);
}

int MenuState::init(){
	int w, h;

	SDL_RenderGetLogicalSize(m_game->renderer(), &w, &h);

	m_drawtex = SDL_CreateTexture(m_game->renderer(), SDL_PIXELTYPE_UNKNOWN,
	SDL_TEXTUREACCESS_TARGET, w, h);

	m_camera = SDL_Rect{0,0,w,h};

	init_menuitems(w, h);

	return 0;
}

void MenuState::update(double dt){
	entityx::ComponentHandle<Drawable> drawable;
	for(entityx::Entity entity: m_entities.entities_with_components(drawable)){
		(void)entity;
		drawable->m_anim.set_y_index(0);
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	entityx::Entity ent;
	bool hit_entity = find_target(x, y, ent);
	if(hit_entity) {
		auto d = ent.component<Drawable>();
		d->m_anim.set_y_index(1);
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			m_game->shutdown();
		}
		else if (e.type == SDL_KEYDOWN) {
			if (e.key.keysym.sym == SDLK_ESCAPE) {
				m_game->shutdown();
			} else if (e.key.keysym.sym == SDLK_F3) {
				m_game->toggle_debug_mode();
			}
		}
		else if (hit_entity && e.type == SDL_MOUSEBUTTONUP){
			if(e.button.button == SDL_BUTTON_LEFT){
				auto c = ent.component<MenuItem>();
				if(c) {
					c->doClick(m_game);
				}
			}
		}
	}

	draw(dt);
}

bool MenuState::find_target(int x, int y, entityx::Entity& ret_entity){
	entityx::ComponentHandle<Position> position;
	entityx::ComponentHandle<Drawable> drawable;
	entityx::ComponentHandle<MenuItem> menuitem;

	for(entityx::Entity entity: m_entities.entities_with_components(position, drawable, menuitem)){
		glm::vec2 pos = position->position();

		if(x >= pos.x && x <= pos.x + drawable->width()){
			if(y >= pos.y && y <= pos.y + drawable->height()){
				ret_entity = entity;
				return true;
			}
		}
	}
	return false;
}

void MenuState::draw(double dt){
	auto renderer = m_game->renderer();

	SDL_SetRenderTarget(renderer, m_drawtex);
	SDL_SetRenderDrawColor(renderer, 0, 100, 200, 0);
	SDL_RenderClear(renderer);
	//return;

	entityx::ComponentHandle<Position> position;
	entityx::ComponentHandle<Drawable> drawable;


	for(entityx::Entity entity: m_entities.entities_with_components(position, drawable)){
		(void) entity;

		glm::vec2 pos = position->position();

		SDL_Rect dest;
		dest.x = pos.x;
		dest.y = pos.y;

		dest.w = drawable->width();
		dest.h = drawable->height();

		SDL_Rect* src;
		AnimTemplate anim = drawable->anim();
		if (anim.frame_width() == 0 || anim.frame_height() == 0) {
			src = nullptr;
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
		(void)tex;(void)dest;(void)src;
		SDL_RenderCopyEx(m_game->renderer(), tex, src, &dest, 0, nullptr, SDL_FLIP_NONE);
	}

	SDL_SetRenderTarget(renderer, nullptr);
	SDL_RenderCopy(renderer, m_drawtex, &m_camera, nullptr);
	SDL_RenderPresent(renderer);
}
