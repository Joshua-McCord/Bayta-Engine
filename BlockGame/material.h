#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Material 
{

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	// ctor/dtor
	Material(glm::vec3 ambient = glm::vec3(1.0f), glm::vec3 diffuse = glm::vec3(1.0f), glm::vec3 specular = glm::vec3(1.0f), float shininess = 0.0f);
	~Material();
};
typedef struct Material Material;


struct GerstnerWave
{
	glm::vec2 direction;
	float amplitude;
	float steepness;
	float frequency;
	float speed;
};
typedef struct GerstnerWave GerstnerWave;

struct GerstnerMaterial 
{
	GerstnerWave waves[9];
	int num_waves;

	// ctor/dtor
	GerstnerMaterial(GerstnerWave gerstner_waves[9] = nullptr, int gerstner_waves_length = 0);
	~GerstnerMaterial();
};

#endif // !MATERIAL_H
