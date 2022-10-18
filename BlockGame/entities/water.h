#pragma once
#ifndef WATER_H
#define WATER_H
#include "entity.h"

struct Water : public Entity
{
	Water() = default;
	Water(Shader* shader, glm::vec2 dimensions, glm::vec3 position = glm::vec3(0.0f), bool is_unit = false);
	~Water();
	GerstnerMaterial gerstner;

	void update(float delta_time);
	void draw(Camera* camera, glm::mat4 projection);
	void save();
};
#endif // !WATER_H