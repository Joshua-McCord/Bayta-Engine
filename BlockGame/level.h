#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#include "mesh.h"
#include "bin/json.hpp"
#include "camera.h"
#include "entities/entity.h"
#include "entities/wall.h"
#include "entities/water.h"
#include "entities/player.h"
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using json = nlohmann::json;

#define RIGHT_VEC (glm::vec3(1, 0, 0))
#define LEFT_VEC (glm::vec3(-1, 0, 0))
#define FORWARD_VEC (glm::vec3(0, 0, -1))
#define BACK_VEC (glm::vec3(0, 0, 1))


enum GRID_TYPES 
{
	GRID_WALL,
	GRID_EMPTY,
};

struct w_m
{
	glm::vec2 dims;
	glm::vec3 pos;
};
typedef struct w_m Water_Model;

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
struct Level
{
	Camera* camera;
	glm::vec3 player_position;
	std::vector<std::vector<GRID_TYPES>> level_grid;
	Water_Model water;

	Level(int level_number = 1);
	~Level();

	void update_player(glm::vec3 direction);

	void print_level_grid();
};
typedef struct Level Level;
#endif // !LEVEL_H


