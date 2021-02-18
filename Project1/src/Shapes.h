#pragma once
#include <gl/glew.h>
#include <vector>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "Shader.h"		
#include <string>
//contains basic shapes for rendering.


enum class ShapeType
{
	QUAD,
	CUBE,
	CUBEMAP,
	SPHERE,
	PLANE
};

class Shapes
{
public:
	Shapes(ShapeType shapetype);
	void addPositions(glm::vec3 position);
	void render(Shader& shader);
	void renderSphere(int rows, int columns, float spacing, Shader& sphere_shader);


private:
	ShapeType shapetype;
	std::vector<glm::vec3> positions_vector;
	unsigned int VAO, VBO ,EBO;
	int numvertices;		//used to store vertices for glDrawArrays
};