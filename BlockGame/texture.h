#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "util/stb_image.h"


struct Texture
{
	unsigned int id;

	Texture(std::string file_location);
	~Texture();
};
typedef struct Texture Texture;

#endif // !TEXTURE_H
