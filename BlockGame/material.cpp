#include "material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

Material::~Material()
{

}

GerstnerMaterial::GerstnerMaterial(GerstnerWave gerstner_waves[9], int gerstner_waves_length)
{
	
	for (int i = 0; i < gerstner_waves_length; ++i)
		this->waves[i] = gerstner_waves[i];

	this->num_waves = gerstner_waves_length;
}

GerstnerMaterial::~GerstnerMaterial()
{
}
