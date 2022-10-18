#include "shader.h"
void check_compile_errors(unsigned int object, std::string type);

bool shader_compile(Shader* shader, const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
	unsigned int s_vertex, s_frag, s_geo;
	// Vertex Shader
	s_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(s_vertex, 1, &vertex_source, NULL);
	glCompileShader(s_vertex);
	check_compile_errors(s_vertex, "VERTEX");
	// Fragment Shader
	s_frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(s_frag, 1, &fragment_source, NULL);
	glCompileShader(s_frag);
	check_compile_errors(s_frag, "FRAGMENT");
	// Geometry Shader
	if (geometry_source != nullptr)
	{
		s_geo = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(s_geo, 1, &geometry_source, NULL);
		glCompileShader(s_geo);
		check_compile_errors(s_geo, "GEOMETRY");
	}
	shader->id = glCreateProgram();
	glAttachShader(shader->id, s_vertex);
	glAttachShader(shader->id, s_frag);
	if (geometry_source != nullptr)
		glAttachShader(shader->id, s_geo);

	glLinkProgram(shader->id);
	check_compile_errors(shader->id, "PROGRAM");

	glDeleteShader(s_vertex);
	glDeleteShader(s_frag);
	if (geometry_source != nullptr)
		glDeleteShader(s_geo);

	return true;
}

void check_compile_errors(unsigned int object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}

void Shader::set_mat4(std::string mat_name, glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(id, mat_name.c_str()), 1, false, glm::value_ptr(matrix));
}

void Shader::set_vec3(std::string vec_name, glm::vec3& vec)
{
	glUniform3f(glGetUniformLocation(id, vec_name.c_str()), vec.x, vec.y, vec.z);
}
void Shader::set_int(std::string int_name, int num)
{
	glUniform1i(glGetUniformLocation(id, int_name.c_str()), num);
}

void Shader::set_float(std::string float_name, float num)
{
	glUniform1f(glGetUniformLocation(id, float_name.c_str()), num);
}

void Shader::set_vec2(std::string vec_name, glm::vec2& vec)
{
	glUniform2f(glGetUniformLocation(id, vec_name.c_str()), vec.x, vec.y);
}

// Definitions
void shader_use(Shader* shader)
{
	glUseProgram(shader->id);
}

void Shader::set_material(Material* material)
{
	set_float("material.shininess", material->shininess);
	set_vec3("material.diffuse", material->diffuse);
	set_vec3("material.ambient", material->ambient);
	set_vec3("material.specular", material->specular);
}

void Shader::set_gerstner_material(GerstnerMaterial* gerstner_material)
{
	set_int("gerstner_waves_length", gerstner_material->num_waves);

	for (int i = 0; i < gerstner_material->num_waves; ++i)
	{
		std::string string = "gerstner_waves[" + std::to_string(i);

		set_vec2(string + "].direction", gerstner_material->waves[i].direction);
		set_float(string + "].amplitude", gerstner_material->waves[i].amplitude);
		set_float(string + "].steepness", gerstner_material->waves[i].steepness);
		set_float(string + "].frequency", gerstner_material->waves[i].frequency);
		set_float(string + "].speed", gerstner_material->waves[i].speed);
	}
}


