#pragma once
#ifndef GAME_H
#define GAME_h
#include <stdlib.h>
#include "renderer.h"
#include "mesh.h"
#include "level.h"

#define WINDOW_WIDTH (1920)
#define WINDOW_HEIGHT (1080)

enum Game_State
{
	EDITOR,
	GAMEPLAY
};

/**
 ** Level Needs the following:
 * 1. Camera
 * 2. Player Position
 * 3. 2-D array of GRID_TYPES
 *
 * The Level is the "Model" class.
 * The Game struct will be most complicated. It will maintain all entities and map level (model) -> rendering (view)
 * game.update will update the level state
 * game.render will render the current state (e.g. draw_mesh(...))
 */
struct Game 
{
	bool keys[1024];

	Renderer* renderer;
	Level* current_level;
	Game_State previous_state;
	Game_State current_state;

	Player_Delta player_delta;

	Game();
	~Game();
	
	void load_current_level();
};
typedef struct Game Game;

void process_input(Game* game, float delta_time);
void process_editor_input(Game* game, float delta_time);
void process_gameplay_input(Game* game, float delta_time);
void update_game(Game* game, float delta_time);
void render_game(Game* game, float delta_time);

#endif // !GAME_H