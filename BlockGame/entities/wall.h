#pragma once
#ifndef WALL_H
#define WALL_H
#include "entity.h"

struct Wall : public Entity
{

	Wall(Shader* shader, glm::vec3 world_position);
	~Wall();

	void update(float delta_time);
	void draw(Camera* camera, glm::mat4 projection);
	void save();
};
#endif // !WALL_H
