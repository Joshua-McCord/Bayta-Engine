#include "player.h"


unsigned int generate_player_vertices();

struct Parsed_Player {
	float anim_curve[4];
	float anim_speed;
};
typedef struct Parsed_Player Parsed_Player;

Parsed_Player parse_player()
{
	Parsed_Player player;

	std::ifstream json_stream("C:/Users/Josh/source/repos/BlockGame/BlockGame/bin/levels/1/view.json");
	json level_json = json::parse(json_stream);

	//// Parse Player Position
	std::vector<float> anim_curve = level_json["player"]["animation_curve"].get<std::vector<float>>();
	for (uint8_t i = 0; i < 4; i++)
		player.anim_curve[i] = anim_curve[i];

	player.anim_speed = level_json["player"]["animation_speed"].get<float>();

	return player;
}

Player::Player(Shader* shader, Texture* texture, glm::vec3 position)
{
	this->world_position = position;

	// Init
	this->mesh;
	this->shader = shader;

	// Drawing
	this->mesh.vao = generate_player_vertices();
	this->mesh.draw_type = TRIANGLE;
	this->mesh.draw_count = 36;

	// Xform
	this->mesh.xform = glm::mat4(1.0f);
	this->mesh.xform = glm::translate(mesh.xform, world_position);

	
	
	// Parse Animation File
	// Load JSON File
	Parsed_Player parsed_player = parse_player();
	std::memcpy(this->animation_curve, parsed_player.anim_curve, 4 * sizeof(float));
	this->animation_speed = parsed_player.anim_speed;

	// Default Animation Time
	this->animation_t = 0.0f;


	// Material
	// Ruby
	this->material = Material(
		glm::vec3(0.1745 	,0.01175 ,	0.01175),	// ambient
		glm::vec3(0.061424 	,0.84136 ,	0.64136),	//diffuse
		glm::vec3(0.727811 	,0.626959, 	0.626959), // Specular
		0.2
	);

	this->texture = texture;

}

Player::~Player()
{
	/* void */
}

void Player::save()
{
	
}

void Player::update(float delta_time)
{
	return;
}

void Player::draw(Camera* camera, glm::mat4 projection)
{
	shader_use(shader);
	shader->set_material(&material);

	shader->set_int("texture1", 0);
	shader->set_int("texture2", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->id);

	draw_mesh(&this->mesh, shader, camera->position, camera_view_matrix(camera), projection);
}

std::string Player::to_string()
{
	std::string player_string = "Player = {";
	player_string += ("Position = " + glm::to_string(world_position));
	return player_string += "}";
}

unsigned int generate_player_vertices()
{
	float vertices[] = {
		// Positions		  // Normals		  // TexCoords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	// Texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return vao;;
}
