#include "SkyBox.h"

SkyBox::SkyBox(Texture& cubemap)
	:cubemap(cubemap)
{

	float skyboxVertices[] =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	VertexBuffer SkyBoxVBO(sizeof(skyboxVertices), skyboxVertices, 3 * sizeof(float));
	this->SkyBoxVBO = SkyBoxVBO;
	this->SkyBoxVBO.addattriblayout(3, GL_FLOAT, GL_FALSE);
	SkyBoxVAO.EnableAttribs(this->SkyBoxVBO);
}


void SkyBox::DrawSkyBox(Shader& skyboxshader,const glm::mat4& CameraViewMat,const glm::mat4& projection)
{
	glDepthFunc(GL_LEQUAL);
	skyboxshader.use();
	glm::mat4 view = glm::mat4(glm::mat3(CameraViewMat));
	skyboxshader.setUniform("view", view); //remove translation to prevent skybox moving
	skyboxshader.setUniform("projection", projection);
	SkyBoxVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);	//36 = sizeof(skyboxvertices) (defined in constrctor of this class)
	glActiveTexture(GL_TEXTURE0);
	cubemap.Bind();
	glDepthFunc(GL_LESS);
}

