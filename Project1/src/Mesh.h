#pragma once
#include "vendor/glm/glm.hpp"
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture_Mesh
{
	unsigned int ID;
	std::string type;
	std::string path;
};

class Mesh
{

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture_Mesh> textures;

	unsigned int VBO, EBO, VAO;

public:
	Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture_Mesh> textures);

	void Draw(Shader& shader);

	void InitMesh();	//called by mesh contructor by default.

};
