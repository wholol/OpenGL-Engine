#pragma once
#include "Shader.h"
#include "Shapes.h"
#include "Texture.h"
#include "SkyBox.h"
#include "Light.h"
#include "Model.h"

class Scene
{

protected:
	Shader& shader;
	Light light;
	Shapes& shape;
	//TODO: CUSTOM MODELS TOO.

public:
	Scene(Shader& shader, Light& light, Shapes& shape);
	void updateScene();
	void renderScene(SkyBox& skybox);			//UI ALLOWS FOR DIFFERENT cubemaps.
};

