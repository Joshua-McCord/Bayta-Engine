#include "camera.h"

void update_camera_vectors(Camera* camera);

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	this->position = position;
	this->world_up = up;
	this->yaw = yaw;
	this->pitch = pitch;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->movement_speed = SPEED;
	this->mouse_sensitivity = SENSITIVITY;
	this->zoom = ZOOM;
	update_camera_vectors(this);
}

Camera::~Camera()
{
	/* void */
}

void update_camera_vectors(Camera* camera)
{
	glm::vec3 front = glm::vec3(
		cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch)),
		sin(glm::radians(camera->pitch)),
		sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch))
	);

	camera->front = glm::normalize(front);
	camera->right = glm::normalize(glm::cross(camera->front, camera->world_up));
	camera->up = glm::normalize(glm::cross(camera->front, camera->right));
}

glm::mat4 camera_view_matrix(Camera* camera)
{
	return glm::lookAt(camera->position, camera->position + camera->front, camera->up);
}

void camera_process_keyboard(Camera* camera, CAMERA_MOVEMENT direction, float delta_time)
{
	float velocity = camera->movement_speed * delta_time * 5;
	if (direction == RIGHT)
		camera->position -= (camera->right * velocity);

	if (direction == LEFT)
		camera->position += (camera->right * velocity);

	if (direction == FORWARD)
		camera->position += (camera->front * velocity);

	if (direction == BACKWARD)
		camera->position -= (camera->front * velocity);

	update_camera_vectors(camera);
}

void camera_process_mouse_movement(Camera* camera, float x_offset, float y_offset, GLboolean constrain_pitch)
{
}

std::string Camera::to_string()
{
	std::string camera_string = "{ \n";
	camera_string += ("Position = " + glm::to_string(position) + ";\n");
	camera_string += ("Up = " + glm::to_string(up) + ";\n");
	camera_string += ("Yaw = " + std::to_string(yaw) + ";\n");
	camera_string += ("Pitch = " + std::to_string(pitch) + ";\n");
	return camera_string + "}";
}
