#pragma once
#include <gl/glew.h>
#include <vector>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "Shader.h"		
//contains basic shapes for rendering.

class Shapes
{
public:
	void renderSphere(int rows, int columns, float spacing, Shader& sphere_shader);


private:




};