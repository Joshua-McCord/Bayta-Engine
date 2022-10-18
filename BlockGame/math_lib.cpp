#include "math_lib.h"

glm::vec3 bezier(glm::vec3 p1, glm::vec3 p2, float t, glm::vec3 p3, glm::vec3 p4)
{
	glm::vec3 a = (1 - t) * (1 - t) * (1 - t) * p1;
	glm::vec3 b = 3 * t * t * (1 - t) * p3;
	glm::vec3 c = 3 * (1 - t) * (1 - t) * t * p2;
	glm::vec3 d = t * t * t * p4;
	return a + b + c + d;
}

glm::vec3 lerp(glm::vec3 a, glm::vec3 b, float t)
{
	glm::vec3 p1 = a;
	glm::vec3 p2 = a + (b - a) * 0.00000001f;
	glm::vec3 p3 = a + (b - a) * 0.99999999f;
	glm::vec3 p4 = b;
	return bezier(p1, p2, t, p3, p4);
}

float clamp(float value, float low, float high)
{
	if (value < low)
		return low;
	if (value > high)
		return high;

	return value;
}