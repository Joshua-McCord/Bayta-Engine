#include "level.h"



glm::vec3 parse_vector(std::vector<float> vec)
{
	return glm::vec3(vec[0], vec[1], vec[2]);
}


Level::Level(int level_number)
{

	// Load JSON File
	std::ifstream json_stream("bin/levels/1/model.json");
	json level_json = json::parse(json_stream);

	// Parse Player Position
	this->player_position = parse_vector(level_json["player_position"].get<std::vector<float>>());

	// Parse Camera
	glm::vec3 position = parse_vector(level_json["camera"]["position"].get<std::vector<float>>());
	glm::vec3 up = parse_vector(level_json["camera"]["up"].get<std::vector<float>>());
	float yaw = level_json["camera"]["yaw"];
	float pitch = level_json["camera"]["pitch"];

	this->camera = new Camera(position, up, yaw, pitch);
	std::cout << camera->to_string();

	// Parse Level Grid
	std::vector<std::vector<std::string>> string_grid = level_json["level_grid"].get<std::vector<std::vector<std::string>>>();
	std::vector<std::vector<std::string>>::iterator row;
	std::vector < std::string > ::iterator col;
	for (row = string_grid.begin(); row != string_grid.end(); ++row)
	{
		std::vector<GRID_TYPES> new_row;
		for (col = row->begin(); col != row->end(); ++col)
		{
			if (*col == "GRID_WALL")
				new_row.push_back(GRID_WALL);
		}
		this->level_grid.push_back(new_row);
	}

	// Parse Water
	std::vector<int> dims = level_json["water"]["dimensions"].get<std::vector<int>>();
	glm::vec2 water_dims = glm::vec2(dims[0], dims[1]);
	glm::vec3 water_pos = parse_vector(level_json["water"]["position"].get<std::vector<float>>());

	water.dims = water_dims;
	water.pos = water_pos;


	// Close file
	json_stream.close();
}

Level::~Level()
{
	delete camera;
}

void Level::print_level_grid()
{
	std::cout << "\n LEVEL_GRID == {\n";
	for (size_t i = 0; i < level_grid.size(); i++)
	{
		for (size_t j = 0; j < level_grid[i].size(); j++)
		{
			if (level_grid[i][j] == GRID_WALL)
			{
				std::cout << "GRID_WALL ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n}\n";
}

void Level::update_player(glm::vec3 direction)
{
	int x = player_position.x;
	int y = player_position.y;
	int z = player_position.z;


	if (direction == RIGHT_VEC)
	{
		while(x < level_grid[z].size() - 1)
			x++;
	}
	if (direction == LEFT_VEC)
	{
		while (x > 0)
			x--;
	}
	if (direction == FORWARD_VEC)
	{
		while (z > 0)
			z--;
	}
	if (direction == BACK_VEC)
	{
		while (z < level_grid.size() - 1)
			z++;
	}
	player_position = glm::vec3(x, y, z);
}
