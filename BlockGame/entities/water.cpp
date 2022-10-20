#include "water.h"

unsigned int generate_tessellated_plane(glm::vec2 dimensions);

struct Parsed_Water {
	Material mat;
	GerstnerMaterial gerst;

};
typedef struct Parsed_Water Parsed_Water;

Parsed_Water parse_water()
{
	Parsed_Water water;

	std::ifstream json_stream("C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/levels/1/view.json");
	json level_json = json::parse(json_stream);

	// Parse Material
	std::vector<float> diffuse	 = level_json["water"]["material"]["diffuse"].get<std::vector<float>>();
	std::vector<float> ambient	 = level_json["water"]["material"]["ambient"].get<std::vector<float>>();
	std::vector<float> specular	 = level_json["water"]["material"]["specular"].get<std::vector<float>>();
	float shininess				 = level_json["water"]["material"]["shininess"].get<float>();

	water.mat.diffuse = glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
	water.mat.ambient = glm::vec3(ambient[0], ambient[1], ambient[2]);
	water.mat.specular = glm::vec3(specular[0], specular[1], specular[2]);
	water.mat.shininess = shininess;


	// Parse Gerstner Material
	water.gerst.num_waves = level_json["water"]["gerstner_material"]["num_waves"].get<int>();
	for (uint8_t i = 0; i < water.gerst.num_waves; ++i)
	{
		std::vector<float> dir = level_json["water"]["gerstner_material"]["wave_" + std::to_string(i)]["direction"].get<std::vector<float>>();
		water.gerst.waves[i].direction = glm::vec2(dir[0], dir[1]);
		water.gerst.waves[i].amplitude = level_json["water"]["gerstner_material"]["wave_" + std::to_string(i)]["amplitude"].get<float>();
		water.gerst.waves[i].frequency = level_json["water"]["gerstner_material"]["wave_" + std::to_string(i)]["frequency"].get<float>();
		water.gerst.waves[i].steepness = level_json["water"]["gerstner_material"]["wave_" + std::to_string(i)]["steepness"].get<float>();
		water.gerst.waves[i].speed	   = level_json["water"]["gerstner_material"]["wave_" + std::to_string(i)]["speed"].get<float>();
	}


	return water;
}

Water::Water(Shader* shader, glm::vec2 dimensions, glm::vec3 position, bool is_unit)
{
	this->world_position = position;
	this->mesh;
	this->shader = shader;
	this->mesh.draw_type = TRIANGLE;
	this->mesh.draw_count = dimensions.x * dimensions.y * 9 * 6;
	this->mesh.vao = generate_tessellated_plane(dimensions);

	glm::mat4 xform = glm::mat4(1.0f);
	xform = glm::translate(xform, world_position);

	if (is_unit)
		xform = glm::scale(xform, glm::vec3(1/dimensions.x, 1, 1/dimensions.y));

	this->mesh.xform = xform;

	Parsed_Water wat = parse_water();

	this->material = wat.mat;
	this->gerstner = wat.gerst;
}

void Water::draw(Camera* camera, glm::mat4 projection)
{
	shader_use(shader);
	shader->set_material(&this->material);
	shader->set_gerstner_material(&this->gerstner);

	draw_mesh(&this->mesh, this->shader, camera->position, camera_view_matrix(camera), projection);
}

void Water::save()
{
	json wall;
	std::ofstream json_stream("C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/levels/1/view.json");
	json_stream << std::setw(4) << wall.dump() << std::endl;
	json_stream.close();
	std::cout << "Saved Wall!" << std::endl;
}

unsigned int generate_tessellated_plane(glm::vec2 dimensions)
{
	int width = dimensions.x;
	int length = dimensions.y;
	int height = 0;

	std::vector<float> vertices;

	for (int x = 0; x < width; x++) {
		for (int z = 0; z < length; z++) {
			// First Triangle
			vertices.push_back((float) x);
			vertices.push_back((float) height);
			vertices.push_back((float) z);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			vertices.push_back((float) x);
			vertices.push_back((float) height);
			vertices.push_back((float) z + 1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			vertices.push_back((float) x + 1.0f);
			vertices.push_back((float) height);
			vertices.push_back((float) z + 1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			// Second Triangle
			vertices.push_back((float) x);
			vertices.push_back((float) height);
			vertices.push_back((float) z);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			vertices.push_back((float) x + 1.0f);
			vertices.push_back((float) height);
			vertices.push_back((float) z);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);

			vertices.push_back((float) x + 1.0f);
			vertices.push_back((float) height);
			vertices.push_back((float) z + 1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
		}
	}
	
	std::cout << "Vert size = " << vertices.size() << std::endl;

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return vao;
}


Water::~Water()
{
	/* void */
}

void Water::update(float delta_time)
{
}
