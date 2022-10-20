#pragma once
#ifndef UI_H
#define UI_H 
#include "level.h"
#include "renderer.h"
#include "math_lib.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct User_Interface
{
	// ctor/dtor
	User_Interface(Level* level, Renderer* renderer);
	~User_Interface();

	Level* level;
	Renderer* renderer;

	void draw_ui();
	void draw_player_options();
	void draw_world_grid_options();
	void draw_water_options();
	void draw_system_options();
};

#endif // !UI_H
