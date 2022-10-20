#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdbool.h>
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "entities/player.h"
#include "entities/wall.h"
#include "level.h"
#include "math_lib.h"
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

typedef struct player_delta {
	glm::vec3 player_original_position;
	glm::vec3 player_new_position;
} Player_Delta;

struct Renderer
{
	// Properties
	glm::mat4 projection;
	std::map<std::string, Shader*> shader_manager;
	std::map<std::string, Texture*> texture_manager;

	bool wireframe = false;

	// ctor/dtor
	Renderer(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT);
	~Renderer();

	// World
	Camera camera;
	Player player;
	std::vector<std::vector<Wall>> entity_grid;
	Water water;

	void set_world(Level* level);
	void update_world(Player_Delta* player_delta);
	void draw_world();

	Texture* load_texture(std::string file_location, std::string texture_name);
	Shader* load_shader(const char* vert_shader_file, const char* frag_shader_file, const char* geo_shader_file, std::string shader_name);

	void save();
};
typedef struct Renderer Renderer;

Shader* load_shader_from_file(const char* vert_shader_file, const char* frag_shader_file, const char* geo_shader_file = nullptr);
Shader* renderer_load_shader(Renderer* renderer, const char* vert_shader_file, const char* frag_shader_file, const char* geo_shader_file, std::string shader_name);


#endif // !RENDERER_H