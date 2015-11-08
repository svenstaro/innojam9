#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

/*
 *  Global config values for easy tweaking and changing
 */

// Maximum orbs spawned on screen
static const int MAX_ORBS = 2;

// variables for borders that restricts movement
static const float RING_INNER = 100.f;
static const float RING_OUTER = 300.f;

//Logic variables for world height and width
static const int WORLD_WIDTH = 1600;
static const int WORLD_HEIGHT = 1200;

//screen width and height
static const int WIDTH = 800;
static const int HEIGHT = 600;

static const float MAX_SPEED = 200.f;

static const bool DEBUG = true;
static const int DEBUG_START_LEVEL = 5; // index of first level

#endif
