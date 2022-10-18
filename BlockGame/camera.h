#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

enum CAMERA_MOVEMENT
{
	FORWARD,
	BACKWARD, 
	LEFT,
	RIGHT
};

// Default Values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.25f;
const float ZOOM = 45.0f;

struct Camera
{
	glm::vec3 position, front, up, right, world_up;
	// euler angles
	float yaw, pitch;
	// camera options
	float movement_speed, mouse_sensitivity, zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	~Camera();

	std::string to_string();
};
typedef struct Camera Camera;


// functions
//void update_camera_vectors(Camera* camera);
glm::mat4 camera_view_matrix(Camera* camera);
void camera_process_keyboard(Camera* camera, CAMERA_MOVEMENT direction, float delta_time);
void camera_process_mouse_movement(Camera* camera, float x_offset, float y_offset, GLboolean constrain_pitch = true);

#endif // !CAMERA_H

