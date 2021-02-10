#pragma once
#include <vector>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

struct Light_Params
{
	glm::vec3 position;
	glm::vec3 Color;
};


class Light
{
public:
	void addPointLight(Light_Params pointlight);	
	void addDirectLight(Light_Params dirlight);
	int numPointLights();
	int numDirectlights();

public:
	std::vector<Light_Params> PointLight_vec;
	std::vector<Light_Params> DirectLight_vec;

};