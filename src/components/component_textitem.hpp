#ifndef TEXTITEM_HPP
#define TEXTITEM_HPP

#include "game.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

struct TextItem : entityx::Component<TextItem> {
	TextItem(std::string text, std::string font_key, SDL_Color color = {0,0,0,255})
	: m_text(text),
		m_font_key(font_key),
		m_color(color){
	}

	std::string text() {
		return m_text;
	}

	std::string font_key() {
		return m_font_key;
	}

	SDL_Color color() {
		return m_color;
	}

private:
	std::string m_text;
	std::string m_font_key;
	SDL_Color m_color;
};

#endif
