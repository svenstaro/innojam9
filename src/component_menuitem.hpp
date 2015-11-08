#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include "game.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

struct MenuItem : entityx::Component<MenuItem> {
	MenuItem(std::string key, std::function<void(Game*)> click)
		: m_key(key), m_click(click){
	}

	std::string key() {
		return m_key;
	}

	void doClick(Game* game) {
		m_click(game);
	}

private:
	std::string m_key;
	std::function<void(Game*)> m_click;
};

#endif
