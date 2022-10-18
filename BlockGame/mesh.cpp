#pragma once
#include "mesh.h"
#include <iostream>

void draw_mesh(Mesh* mesh, Shader* shader, glm::vec3 viewPos, glm::mat4 view, glm::mat4 projection)
{
	glm::mat4 model = mesh->xform; 
	glm::mat4 u_mvp = projection * view * model;

	//TODO Move this to renderer
	glm::vec3 light_pos = glm::vec3(5.0f, 20.0f, 0.0f);
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 object_color = glm::vec3(1.0f, 0.5f, 0.31f);

	shader_use(shader);
	shader->set_mat4("u_model", model);
	shader->set_mat4("u_view", view);
	shader->set_mat4("u_projection", projection);
	shader->set_vec3("viewPos", viewPos);
	shader->set_vec3("lightPos", light_pos);
	shader->set_vec3("lightColor", light_color);
	shader->set_vec3("objectColor", object_color);
	
	//shader->set_vec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));

	shader->set_float("u_time", glfwGetTime());
	//shader_set_mat4(shader,  "u_mvp", u_mvp);
	shader->set_mat4("u_mvp", u_mvp);

	unsigned int vao = mesh->vao;
	glBindVertexArray(vao);
	if (mesh->draw_type == TRIANGLE)
	{
		glDrawArrays(GL_TRIANGLES, 0, mesh->draw_count);
	}
	else if (mesh->draw_type == ELEMENT)
	{
		glDrawElements(GL_TRIANGLES, mesh->draw_count, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}