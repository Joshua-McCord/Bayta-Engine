#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "entity.h"

enum PLAYER_STATE {
	IDLE,
	ANIMATING
};

struct Player : public Entity
{
	// Methods
	Player() = default;
	Player(Shader* shader, Texture* texture, glm::vec3 world_position);
	~Player();
	
	void update(float delta_time);
	void draw(Camera* camera, glm::mat4 projection);
	void save();
	std::string to_string();

	// Variables
	PLAYER_STATE current_state = IDLE;
	// Animation
	float animation_curve[4] = { 0.0f, 0.00001f, 0.99999f, 1.0f };
	float animation_speed = 0.06;
	float animation_t = 0;
};
#endif // !PLAYER_H