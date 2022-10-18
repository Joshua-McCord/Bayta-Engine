#include "renderer.h"
#include "bezier.hpp"

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

void draw_water_options(Water* water)
{
	bool p = true;
	bool* p_open = &p;


	GerstnerMaterial& gerstner_material = water->gerstner;
	ImGui::SetNextWindowSize(ImVec2(400, 100), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Example: Simple layout", p_open, ImGuiWindowFlags_MenuBar))
	{
		ImGui::SetWindowFontScale(1.5f);
		// Left
		static int selected = 0;
		{
			ImGui::BeginChild("left pane", ImVec2(150, 0), true);
			for (int i = 0; i < gerstner_material.num_waves; ++i)
			{
				// FIXME: Good candidate to use ImGuiSelectableFlags_SelectOnNav
				char label[128];
				sprintf_s(label, "Wave %d", i + 1);
				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();

		// Right
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
			ImGui::Text("Wave %d", selected);
			ImGui::Separator();
			// Direction

			//ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
			ImGui::SliderFloat2("Dir.", &gerstner_material.waves[selected].direction.x, -2, 2);
			ImGui::SliderFloat("Ampl.", &gerstner_material.waves[selected].amplitude, -2, 2);
			ImGui::SliderFloat("Freq.", &gerstner_material.waves[selected].frequency, -2, 2);
			ImGui::SliderFloat("Spd.", &gerstner_material.waves[selected].speed, -2, 2);
			ImGui::SliderFloat("Stpns.", &gerstner_material.waves[selected].steepness, -2, 2);
			
			 
			
		}
		ImGui::SameLine();
		{
			ImGui::Separator();
			//ImGui::SliderFloat3("Diffuse", &water->material.diffuse.x, 0, 1);
			ImGui::ColorPicker4("Diffuse", &water->material.diffuse.x);
			ImGui::ColorPicker4("Specular", &water->material.specular.x);
			ImGui::ColorPicker4("Ambient", &water->material.ambient.x);
			ImGui::SliderFloat("Shininess", &water->material.shininess, 0, 5);

			ImGui::EndChild();
			if (ImGui::Button("Revert")) {}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {}
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}

void Renderer::draw_ui()
{
	// Start a new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Draw UI 
	// Create a window called "My First Tool", with a menu bar.

	ImGui::Begin("Player Controls");
	ImGui::SetWindowFontScale(1.5f);

	if (ImGui::MenuItem("Save", "Ctrl+S")) { save(); }
	ImGui::Bezier("Anim Curve", player.animation_curve);
	ImGui::SliderFloat("Speed", &player.animation_speed, 0.0f, 0.5f);

	ImGui::End();

	ImGui::Begin("Misc.");
	ImGui::SetWindowFontScale(1.5f);
	ImGui::Checkbox("Wireframe Rendering", &wireframe);
	ImGui::End();


	draw_water_options(&water);

	

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::save()
{
	player.save();
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
