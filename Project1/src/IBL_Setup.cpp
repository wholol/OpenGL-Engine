#include "IBL_Setup.h"


IBL_Setup::IBL_Setup(const std::string& img_name, int diffuse_irr_texWidth, int diffuse_irr_texHeight,
					int specular_texWidth, int specular_texHeight, int BRDF_intergral_texWidth, int BRDF_intergral_texHeight)

	:diffuse_irr_texWidth(diffuse_irr_texWidth) , diffuse_irr_texHeight(diffuse_irr_texHeight), BRDF_intergral_texWidth(BRDF_intergral_texWidth)
	,BRDF_intergral_texHeight(BRDF_intergral_texHeight) , specular_texHeight(specular_texHeight) , specular_texWdith(specular_texWidth)
	,img_name(img_name)
{
	//generate the diffuse and specular IBL textures for sampling.
	HDR_to_cubemap();
	diffuse_irradiance_init();
	specular_mipmap_init();
	BRDF_intergation_map_init();
}

Texture & IBL_Setup::getRenderCubeMap()
{
	return cubemap;
}

void IBL_Setup::diffuse_irradiance_Bind()
{
	diffuse_irradiance_cubemap.Bind();
}

void IBL_Setup::BRDF_intergration_map_Bind()
{
	BRDF_intergration_map.Bind();
}

void IBL_Setup::specular_cubemap_Bind()
{
	specular_prefilter_cubemap.Bind();
}


void IBL_Setup::HDR_to_cubemap()
{
	//load HDR
	HDRTexture.loadHDR(img_name);

	//generate null cubemap
	cubemap.genCubeMap(diffuse_irr_texWidth, diffuse_irr_texHeight);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, diffuse_irr_texWidth, diffuse_irr_texHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	equi_2_cubemap_shader.use();					//use the conversion shader
	equi_2_cubemap_shader.setUniform("projection", captureProjection);
	equi_2_cubemap_shader.setUniform("equirectangularMap", 0);
	glActiveTexture(GL_TEXTURE0);	//sample HDR texture in the shader
	HDRTexture.Bind();

	glViewport(0, 0, diffuse_irr_texWidth, diffuse_irr_texHeight);

	//convert HDR image into a cubemap
	for (int i = 0; i < 6; ++i)
	{
		equi_2_cubemap_shader.setUniform("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap.getTexID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void IBL_Setup::diffuse_irradiance_init()
{
	
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//CONVOLUTE CUBEMAP
	//now that we have a cubemap from the HDR, perform convolution on the cubemap.
	diffuse_irradiance_cubemap.genCubeMap(32, 32);
	diff_convolution_shader.use();	//use the convolution shader
	diff_convolution_shader.setUniform("projection", captureProjection);
	diff_convolution_shader.setUniform("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	cubemap.Bind();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glViewport(0, 0, 32, 32);
	for (int i = 0u; i < 6; ++i)
	{
		diff_convolution_shader.setUniform("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, diffuse_irradiance_cubemap.getTexID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderCube();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


void IBL_Setup::BRDF_intergation_map_init()
{
	BRDF_intergration_map.genTexture(BRDF_intergral_texWidth, BRDF_intergral_texHeight);
	BRDF_intergrate_shader.use();
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, BRDF_intergral_texWidth, BRDF_intergral_texHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDF_intergration_map.getTexID(), 0);
	glViewport(0, 0, BRDF_intergral_texWidth, BRDF_intergral_texHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


void IBL_Setup::specular_mipmap_init()
{
	//generate null cube mip map
	specular_prefilter_cubemap.genCubeMap(specular_texWdith, specular_texHeight, true);	

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};
	
	specular_pre_filtered_shader.use();
	specular_pre_filtered_shader.setUniform("environmentMap", 0);
	glActiveTexture(GL_TEXTURE0);
	cubemap.Bind();
	specular_pre_filtered_shader.setUniform("projection", captureProjection);
	
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	
	const int num_mipmaps = 5;
	
	for (int mip = 0; mip < num_mipmaps; ++mip)
	{
		unsigned int mipmap_width = specular_texWdith * std::pow(0.5, mip);
		unsigned int mipmap_height = specular_texHeight * std::pow(0.5, mip);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipmap_width, mipmap_height);
		glViewport(0,0,mipmap_width, mipmap_height);
		
		float roughness = (float)mip / (float)(num_mipmaps - 1);
		specular_pre_filtered_shader.setUniform("roughness", roughness);

		for (int i = 0; i < 6; ++i)
		{
			specular_pre_filtered_shader.setUniform("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, specular_prefilter_cubemap.getTexID(), mip);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

