#include "game.h"

Game::Game() {

	// Model, View, UI
	renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	current_level = new Level(1);
	user_interface = new User_Interface(current_level, renderer);

	// I don't like this
	this->player_delta.player_original_position = current_level->player_position;
	this->player_delta.player_new_position = current_level->player_position;

	load_current_level();

	memset(keys, 0, sizeof(keys));
	previous_state = EDITOR;
	current_state = EDITOR;

	std::cout << "Game Initialized\n";
}

void Game::load_current_level()
{
	renderer->set_world(current_level);
}

Game::~Game()
{
	delete renderer;
	delete current_level;
}

void process_input(Game* game, float delta_time)
{
	// Switch between engine and gameplay
	if (game->keys[GLFW_KEY_E] || game->keys[GLFW_KEY_P])
	{
		game->previous_state = game->current_state;
		if(game->keys[GLFW_KEY_E])
			game->current_state = EDITOR;

		if (game->keys[GLFW_KEY_P])
			game->current_state = GAMEPLAY;
	}
	// Branch into the current game state
	else {
		if (game->current_state == EDITOR)
		{
			process_editor_input(game, delta_time);
		}
		else if (game->current_state == GAMEPLAY)
		{
			process_gameplay_input(game, delta_time);
		}
	}
}

void process_editor_input(Game* game, float delta_time)
{
	Camera* camera = game->current_level->camera;
	if (game->keys[GLFW_KEY_W])
		camera_process_keyboard(camera, FORWARD, delta_time);
	if (game->keys[GLFW_KEY_S])
		camera_process_keyboard(camera, BACKWARD, delta_time);
	if (game->keys[GLFW_KEY_A])
		camera_process_keyboard(camera, LEFT, delta_time);
	if (game->keys[GLFW_KEY_D])
		camera_process_keyboard(camera, RIGHT, delta_time);
}

// Update Level Model
void process_gameplay_input(Game* game, float delta_time)
{
	Level* level = game->current_level;
;
	

	if (game->renderer->player.current_state == IDLE)
	{
		game->player_delta.player_original_position = level->player_position;

		if (game->keys[GLFW_KEY_D])
			level->update_player(RIGHT_VEC);
		else if (game->keys[GLFW_KEY_A])
			level->update_player(LEFT_VEC);
		else if (game->keys[GLFW_KEY_W])
			level->update_player(FORWARD_VEC);
		else if (game->keys[GLFW_KEY_S])
			level->update_player(BACK_VEC);

		game->player_delta.player_new_position = level->player_position;
	}
}

// Map Level to Renderer
void update_game(Game* game, float delta_time)
{
	game->renderer->update_world(&game->player_delta);
}

// Draw current renderer state
void render_game(Game* game, float delta_time)
{
	game->renderer->draw_world();
	game->user_interface->draw_ui();
}
