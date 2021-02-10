#include "Light.h"

void Light::addPointLight(Light_Params pointlight)
{
	PointLight_vec.emplace_back(pointlight);
}

void Light::addDirectLight(Light_Params dirlight)
{
	DirectLight_vec.emplace_back(dirlight);
}

int Light::numPointLights()
{
	return PointLight_vec.size();
}

int Light::numDirectlights()
{
	return DirectLight_vec.size();
}
