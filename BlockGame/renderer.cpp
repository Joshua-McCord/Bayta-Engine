#include "renderer.h"


Renderer::Renderer(unsigned int WINDOW_WIDTH, unsigned int WINDOW_HEIGHT)
{
	projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	renderer_load_shader(
		this,
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/wall.vert",
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/wall.frag",
		nullptr,
		"wall_shader"
	);

	renderer_load_shader(
		this,
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/player.vert",
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/player.frag",
		nullptr,
		"player_shader"
	);

	renderer_load_shader(
		this,
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/water/gerstner_water.vert",
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/water/gerstner_water.frag",
		"C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/shaders/water/gerstner_water.geom",
		"water_shader"
	);

	load_texture("C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/textures/rock_texture.jpg", "rock_texture");
	load_texture("C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/textures/rock_normal_map.jpg", "rock_normal_map");
}

Renderer::~Renderer()
{
	/* void */
}

void Renderer::set_world(Level* level)
{
	this->camera = *level->camera;
	player = Player(shader_manager["player_shader"], texture_manager["rock_normal_map"], level->player_position);
	for (size_t i = 0; i < level->level_grid.size(); ++i)
	{
		std::vector<Wall> new_row;
		for (size_t j = 0; j < level->level_grid[i].size(); ++j)
		{
			if (level->level_grid[i][j] == GRID_WALL)
				new_row.push_back(Wall(shader_manager["wall_shader"], glm::vec3(j, 0, i)));
		}
		this->entity_grid.push_back(new_row);
	}
	water = Water(shader_manager["water_shader"], level->water.dims, level->water.pos);
}

void Renderer::update_world(Player_Delta* player_delta)
{
	/*Player& player = player;*/
	glm::vec3& orig_pos = player_delta->player_original_position;
	glm::vec3& new_pos = player_delta->player_new_position;

	if (orig_pos != new_pos)
		player.current_state = ANIMATING;


	if (player.current_state == ANIMATING)
	{
		player.mesh.xform = glm::mat4(1.0f);
		player.mesh.xform = glm::translate(player.mesh.xform, bezier(
			orig_pos,
			(orig_pos + (new_pos - orig_pos) * player.animation_curve[1]),
			clamp(player.animation_t, 0.0f, 1.0f),
			(orig_pos + (new_pos - orig_pos) * player.animation_curve[2]),
			new_pos
		));

		// We are at the new position, break
		if (player.animation_t > 1.0f)
		{
			player.current_state = IDLE;
			player.animation_t = 0.0;
			orig_pos = player_delta->player_new_position;
			new_pos = player_delta->player_new_position;
			return;
		}
		player.animation_t += player.animation_speed;
	}
}

void Renderer::draw_world()
{
	if(wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw Player
	player.draw(&camera, projection);
	
	// Draw World Grid
	std::vector<std::vector<Wall>>::iterator row;
	std::vector<Wall>::iterator col;
	for (row = entity_grid.begin(); row != entity_grid.end(); ++row)
	{
		for (col = row->begin(); col != row->end(); ++col)
		{
			Wall wall = *col;
			wall.draw(&camera, projection);
		};
	}

	// Draw Water
	water.draw(&camera, projection);
}

void Renderer::save()
{
	json view;
	// Save Player
	view["player"]["animation_speed"] = player.animation_speed;
	view["player"]["animation_curve"] = player.animation_curve;

	// Save Water Material
	std::vector<float> water_diff = { water.material.diffuse.r, water.material.diffuse.g, water.material.diffuse.b };
	float water_amb[3] = { water.material.ambient.r, water.material.ambient.g, water.material.ambient.b };
	float water_spec[3] = { water.material.specular.r, water.material.specular.g, water.material.specular.b };

	

	view["water"]["material"]["diffuse"] = water_diff;
	view["water"]["material"]["ambient"] = water_amb;
	view["water"]["material"]["specular"] = water_spec;
	view["water"]["material"]["shininess"] = water.material.shininess;

	// Save Water Gerstner Props
	view["water"]["gerstner_material"]["num_waves"] = water.gerstner.num_waves;
	for (uint8_t i = 0; i < water.gerstner.num_waves; i++)
	{
		float wave_dir[2] = { water.gerstner.waves[i].direction.x, water.gerstner.waves[i].direction.y };
		view["water"]["gerstner_material"]["wave_" + std::to_string(i)]["amplitude"] = water.gerstner.waves[i].amplitude;
		view["water"]["gerstner_material"]["wave_" + std::to_string(i)]["direction"] = wave_dir;
		view["water"]["gerstner_material"]["wave_" + std::to_string(i)]["frequency"] = water.gerstner.waves[i].frequency;
		view["water"]["gerstner_material"]["wave_" + std::to_string(i)]["speed"] = water.gerstner.waves[i].speed;
		view["water"]["gerstner_material"]["wave_" + std::to_string(i)]["steepness"] = water.gerstner.waves[i].steepness;
	}
	
	// Dump JSON
	std::ofstream json_stream("C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/levels/1/view.json");
	json_stream << std::setw(4) << view.dump() << std::endl;
	json_stream.close();
	std::cout << "Saved Renderer!" << std::endl;
}

// Shaders
Shader* renderer_load_shader(Renderer* renderer, const char* vert_shader_file, const char* frag_shader_file, const char* geo_shader_file, std::string shader_name)
{
	renderer->shader_manager[shader_name] = load_shader_from_file(vert_shader_file, frag_shader_file, geo_shader_file);
	return renderer->shader_manager[shader_name];
}

Shader* load_shader_from_file(const char* vert_shader_file, const char* frag_shader_file, const char* geo_shader_file)
{
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;
	try
	{
		// open files
		std::ifstream vertexShaderFile(vert_shader_file);
		std::ifstream fragmentShaderFile(frag_shader_file) ;
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertex_code = vShaderStream.str();
		fragment_code = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geo_shader_file != nullptr)
		{
			std::cout << "Reading Geo\n";
			std::ifstream geometryShaderFile(geo_shader_file);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometry_code = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertex_code.c_str();
	const char* fShaderCode = fragment_code.c_str();
	const char* gShaderCode = geometry_code.c_str();
	Shader* shader = new Shader;
	shader_compile(shader, vShaderCode, fShaderCode, geo_shader_file != nullptr ? gShaderCode : nullptr);
	return shader;
}

// Textures
Texture* Renderer::load_texture(std::string file_location, std::string texture_name)
{
	texture_manager[texture_name] = new Texture(file_location);
	return texture_manager[texture_name];
}
