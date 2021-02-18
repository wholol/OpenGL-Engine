#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox(Texture& cubemap);
	void DrawSkyBox(Shader& skyboxshader , const glm::mat4& CameraViewMat, const glm::mat4& projection);
	void setCubeMapTexture(Texture& other);

private:
	Texture cubemap;
	VertexArray SkyBoxVAO;
	VertexBuffer SkyBoxVBO;
};