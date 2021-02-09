#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox(CustomTexture& cubemap);
	void DrawSkyBox(Shader& skyboxshader , const glm::mat4& CameraViewMat, const glm::mat4& projection);

private:
	CustomTexture& cubemap;
	VertexArray SkyBoxVAO;
	VertexBuffer SkyBoxVBO;
};