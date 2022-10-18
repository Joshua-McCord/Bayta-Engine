#pragma once
#ifndef ENTITY_H
#define ENTITY_H

// System Includes
#include <glm/vec3.hpp>

// Model Includes
#include "../mesh.h"
#include "../shader.h"
#include "../camera.h"
#include "../material.h"
#include "../texture.h"
#include "../bin/json.hpp"

using json = nlohmann::json;

enum ENTITY_TYPE {
	WALL, 
	PLAYER,
};


struct Entity {
	glm::vec3 model_position;
	glm::vec3 world_position;
	Mesh mesh;
	Material material;
	Shader* shader;
	Texture* texture;

	int id;

	virtual void update(float delta_time) = 0;
	virtual void draw(Camera* camera, glm::mat4 projection) = 0;
	virtual void save() = 0;
};
typedef struct Entity Entity;
#endif // !ENTITY_H
