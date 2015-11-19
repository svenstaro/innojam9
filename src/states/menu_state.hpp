#ifndef MENU_STATE_MAIN_HPP
#define MENU_STATE_MAIN_HPP

#include "strapon/state/state.hpp"

#include "entityx/entityx.h"

#include "game.hpp"

class MenuState : public State {
public:
	MenuState(Game *game);
	~MenuState();
	int init() override;
	void update(double dt) override;

protected:
	virtual void init_menuitems(int w, int h) = 0;

	bool find_target(int x, int y, entityx::Entity& ret_entity);
	void draw(double dt);

	Game *m_game;

	SDL_Rect m_camera;
	SDL_Texture *m_drawtex;

	entityx::EventManager m_events;
	entityx::EntityManager m_entities{m_events};
	entityx::SystemManager m_systems{m_entities, m_events};
};

#endif
