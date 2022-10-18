#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <map>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "material.h"


struct Shader
{
	unsigned int id;
	
	void set_material(Material* material);
	void set_gerstner_material(GerstnerMaterial* gerstner_material);

	void set_mat4(std::string mat_name, glm::mat4& matrix);
	void set_vec3(std::string vec_name, glm::vec3& vec);
	void set_vec2(std::string vec_name, glm::vec2& vec);
	void set_int(std::string int_name, int num);
	void set_float(std::string float_name, float num);

};
typedef struct Shader shader;

// Declarations
//void shader_set_mat4(Shader* shader, std::string mat_name, glm::mat4& matrix);
//void shader_set_vec3(Shader* shader, std::string vec_name, glm::vec3 vec);
//void shader_set_int(Shader* shader, std::string int_name, int num);
//void shader_set_float(Shader* shader, std::string float_name, float num);
//void shader_set_vec2(Shader* shader, std::string vec_name, glm::vec2 vec);

void shader_use(Shader* shader);
bool shader_compile(Shader* shader, const char* vertex_source, const char* fragment_source, const char* geometry_source);

#endif // !SHADER_H
