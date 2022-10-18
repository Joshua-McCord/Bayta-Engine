#pragma once
#ifndef BLOCK_MATH_H
#define BLOCK_MATH_H
#include <glm/vec3.hpp>

glm::vec3 bezier(glm::vec3 p1, glm::vec3 p2, float t, glm::vec3 p3, glm::vec3 p4);

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t);

float clamp(float value, float low, float high);

#endif // !MATH_HPP
