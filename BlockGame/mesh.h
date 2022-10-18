#pragma once
#ifndef MESH_H
#define MESH_H
#include "shader.h"

#include <glm/mat4x4.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h> 
#include <vector>
#include <string>

enum MESH_DRAW_TYPE
{
	TRIANGLE,
	ELEMENT,
};

typedef struct Mesh Mesh;
struct Mesh
{
	unsigned int vao;
	MESH_DRAW_TYPE draw_type;
	unsigned int draw_count;

	glm::mat4 xform;
};
typedef struct Mesh Mesh;

void draw_mesh(Mesh* mesh, Shader* shader, glm::vec3 viewPos, glm::mat4 view, glm::mat4 projection);

#endif // !MESH_H
