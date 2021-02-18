#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include "camera.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "Model.h"
#include "SkyBox.h"
#include "Light.h"
#include "Shapes.h"

double lastX = 400, lastY = 300;	//initialize mouse pos to center of screen
bool firstMouse = true;

float deltatime = 0.0f;
float lastframe = 0.0f;

void renderCube();

glm::vec3 camerapos = glm::vec3(0.0f, 0.0f, 3.0f);
camera cam(camerapos);


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.movecamera(FORWARD, deltatime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.movecamera(BACKWARD, deltatime);
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.movecamera(LEFT, deltatime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.movecamera(RIGHT, deltatime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	cam.rotatecamera(xoffset, yoffset);
}


int main()
{
	if (!glfwInit())	//iiitliaze glfw library
	{
		std::cout << "failed to iitialize GLFW." << "\n";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//openGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	GLFWwindow* window = glfwCreateWindow(1200,1000,"test",nullptr,nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); 

	if (glewInit() != GLEW_OK) {	//*MUST BE AFTER HAVING A VALID CONTEXT*//
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);















	//SHADOW MAPPING 
	//Shader depthshader("assets/shaders/depthmap.vs", "assets/shaders/depthmap.fs");				//depth shader.
	//Shader depthtoquad("assets/shaders/depth_to_quad.vs", "assets/shaders/depth_to_quad.fs");	//render depth map to quad
	//unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	//unsigned int ShadowMapFBO;
	//glGenFramebuffers(1, &ShadowMapFBO);
	//unsigned int depthmap;
	//glGenTextures(1, &depthmap);
	//glBindTexture(GL_TEXTURE_2D, depthmap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//float borderColor[] = { 1.0f , 1.0f , 1.0f , 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthmap, 0);	//attach depthmap to rambeuggeer
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	////SHADOW MAP: LIGHT
	//float near_p = 1.0f, far_p = 7.5f;
	//glm::mat4 light_ortho = glm::ortho(-10.0f, 10.0f, near_p, far_p);
	//
	//glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	//skyboxtexture setup
	std::vector<std::string> skyboxjpg;
	skyboxjpg.emplace_back("right.jpg");
	skyboxjpg.emplace_back("left.jpg");
	skyboxjpg.emplace_back("top.jpg");
	skyboxjpg.emplace_back("bottom.jpg");
	skyboxjpg.emplace_back("front.jpg");
	skyboxjpg.emplace_back("back.jpg");

	Texture skyboxtexture;
	skyboxtexture.loadCubeMap(skyboxjpg);
	Shader skyboxshader("assets/shaders/skyvert.vs","assets/shaders/skyfrag.fs");
	SkyBox sky(skyboxtexture);
	
	
	//model setup
	//Model bag("backpack/backpack.obj");
	//Shader bagshader("assets/shaders/bagvert.vs", "assets/shaders/bagfrag.fs");


	//light setup. TODO: make singleton
	Light light;
	glm::vec3 light_color(300.0f, 300.0f, 300.0f);
	light.addPointLight({ glm::vec3(-10.0f , 10.0f , 10.0f) , light_color });
	light.addPointLight({ glm::vec3(10.0f , 10.0f , 10.0f) , light_color });
	light.addPointLight({ glm::vec3(-10.0f , -10.0f , 10.0f) , light_color });
	light.addPointLight({ glm::vec3(10.0f , -10.0f , 10.0f) , light_color });

	//sphere
	Shapes spheres(ShapeType::SPHERE);
	Shader pbrshader("assets/shaders/pbr.vs" , "assets/shaders/pbr.fs" );

#define IBL_enable 1

#if IBL_enable
	//PBR IBL
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	unsigned int cubeFBO, cubeRBO;
	glGenFramebuffers(1, &cubeFBO);
	glGenRenderbuffers(1, &cubeRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, cubeRBO);

	Texture hdrTexture;		//load hdr texture
	hdrTexture.loadHDR("newport_loft.hdr");
	Texture envCubeMap;		//set up an empty cube map with no data
	
	
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


	
	//STEP 1: CONVERT HDR TEXTURE (EQUIRECT MAP) INTO CUBE
	Shader equi_2_cubemap_shader("assets/shaders/equi_2_cubemap.vs", "assets/shaders/equi_2_cubemap.fs");
	//equi_2_cubemap_shader.use();
	//equi_2_cubemap_shader.setUniform("projection", captureProjection);
	//equi_2_cubemap_shader.setUniform("equirectangularMap", 0);	//sampler2D
	//glActiveTexture(GL_TEXTURE0);
	//hdrTexture.Bind();		//hdr texture == equirectangularMap.


	hdrTexture.HDR_to_CubeMap("newport_loft.hdr", 512, 512, envCubeMap, equi_2_cubemap_shader);


	//STEP 2: sample hdr texture onto a cube, and store it into a cubemap. rendering is done on another FBO.
	//glViewport(0, 0, 512, 512);
	//glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	
	//for (int i = 0u; i < 6; ++i)	//bind the cubemap to the framebuffer. in a way, we "fill" the envcube map, which was set to null data initially.
	//{
	//	equi_2_cubemap_shader.setUniform("view", captureViews[i]);
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubeMap.getTexID(), 0);
		//render cube to the frame buffer.
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	renderCube();
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);	//unbind FBO.
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	
	Texture HDRCubeMap;		//cubemap to map HDR image on
	Texture ConvolutedHDR;
	//ConvolutedHDR.HDR_to_CubeMap("Newport_Loft_Env.hdr", 512, 512, HDRCubeMap,equi_2_cubemap_shader);	//convet HDR iamge into the cubemap.
	
	ConvolutedHDR.loadHDR("newport_loft.hdr");	//load blurred HDR
	
	
	equi_2_cubemap_shader.use();
	equi_2_cubemap_shader.setUniform("projection", captureProjection);
	equi_2_cubemap_shader.setUniform("equirectangularMap", 0);	//sampler2D
	glActiveTexture(GL_TEXTURE0);
	ConvolutedHDR.Bind();
	
	glViewport(0, 0, 512, 512);
	unsigned int FBO_2ND, RBO_2ND;
	glGenFramebuffers(1, &FBO_2ND);
	glGenRenderbuffers(1, &RBO_2ND);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_2ND);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_2ND);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO_2ND);


	glBindFramebuffer(GL_FRAMEBUFFER, FBO_2ND);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_2ND);
	
	for (int i = 0u; i < 6; ++i)	//bind the cubemap to the framebuffer. in a way, we "fill" the envcube map, which was set to null data initially.
	{
		equi_2_cubemap_shader.setUniform("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, HDRCubeMap.getTexID(), 0);
		//render cube to the frame buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//unbind FBO.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	

	//SPECULAR IBL:
	//STEP1 : GENERATE PREFILTERED ENVIONMENT MIPMAP WITH VARYING ROUGHNESS.
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);			//note that the mipmap is also nullptr now.

	Shader prefilter_specularIBL("assets/shaders/equi_2_cubemap.vs", "assets/shaders/prefilter_specularIBL.fs");
	prefilter_specularIBL.use();
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);	//to render to cubemap
	int max_mipmaps = 5;
	for (int i = 0; i < max_mipmaps; ++i)
	{
		float roughness = float(i) / float(max_mipmaps);
		prefilter_specularIBL.setUniform("roughness", roughness);

		//resizeing mipmaps: rather trivial, each image is halved for every mipmap iteration.
		unsigned int mipWidth = 128 * std::pow(0.5, i);
		unsigned int mipHeight = 128 * std::pow(0.5, i);
		glBindRenderbuffer(GL_FRAMEBUFFER, cubeRBO);
		for (int j = 0; j < 6; ++j)
		{
			prefilter_specularIBL.setUniform("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, prefilterMap, 0);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	//unbind FBO.
	

	//convolute the enviormnet cubemap
	//Texture irradianceCubeMap;
	//irradianceCubeMap.genCubeMap(32,32);
	//Shader convoluteShader("assets/shaders/equi_2_cubemap_shader.vs", "assets/shaders/convolute_cubemap.fs");
	//convoluteShader.use();
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	//
	//
    //convoluteShader.setUniform("projection", captureProjection);
	//convoluteShader.setUniform("environmentMap", 0);
	//glActiveTexture(GL_TEXTURE0);
	//envCubeMap.Bind();
	//glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	//glViewport(0, 0,32, 32);
	//for (int i = 0u; i < 6; ++i)	//bind the cubemap to the framebuffer. in a way, we "fill" the envcube map, which was set to null data initially.
	//{
	//	convoluteShader.setUniform("view", captureViews[i]);
	//	
	//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceCubeMap.getTexID(), 0);
	//	//render cube to the frame buffer.
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	//	renderCube();
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);	//unbind FBO.


	SkyBox IBL(envCubeMap);		//skybox = for rendering purposes.
#endif

	float t = 0.0f;
	
	//Shader cubeshader("assets/shaders/cubevertex.vs", "assets/shaders/cubefragment.fs");	//for drawing ad shading cube objects
	//Shader lightcube("assets/shaders/lightvertex.vs","assets/shaders/lightfrag.fs");	//for drawing light cubes

	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);



	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		float currentFrame = glfwGetTime();
		deltatime = currentFrame - lastframe;
		lastframe = currentFrame;
		processInput(window);

		
		
		//imgui::model matrix here (translation/rotation)
		float translateX = 0.0f;
		float translateY = 0.0f;
		float translateZ = 0.0f;

		float rotateX = 0.0f;
		float rotateY = 0.0f;
		float rotateZ = 0.0f;

		glm::mat4 model = glm::mat4(1.0f);

		//view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = cam.GetViewMat();
		
		
		//if imgui::pbr workflow
		pbrshader.use();
		pbrshader.setUniform("view", view);
		pbrshader.setUniform("camPos", cam.camerapos);
		pbrshader.setUniform("projection", projection);

		
		float ao = 1.0f;		//hould be slider
		glm::vec3 albedo = glm::vec3(0.5f, 0.0f, 0.0f);	//slider
		float metallic = 1.0f;	//slider
		float roughness = 0.06f;	//slider
		bool isTextured = false; //checker
		
		if (isTextured)
		{
			pbrshader.setUniform("albedoMap", 0);
			//TODO:bind tex
			glActiveTexture(GL_TEXTURE0);
			pbrshader.setUniform("normalMap", 1);
			//bind tex
			glActiveTexture(GL_TEXTURE1);
			pbrshader.setUniform("metllicMap", 2);
			//bind tex
			glActiveTexture(GL_TEXTURE2);
			pbrshader.setUniform("roughnessMap", 3);
			//bind tex
			glActiveTexture(GL_TEXTURE3);
			pbrshader.setUniform("aoMap", 4);
			//bind tex
			glActiveTexture(GL_TEXTURE4);
			
		}
		else
		{
			pbrshader.setUniform("ao", ao);
			pbrshader.setUniform("albedo", albedo);
			pbrshader.setUniform("metallic", metallic);
			pbrshader.setUniform("roughness", roughness);
			pbrshader.setUniform("isTextured", isTextured);
		}
	
		
		for (int i = 0; i < light.numPointLights(); ++i)
		{
			pbrshader.setUniform("light[" + std::to_string(i) + "].LightPos", light.PointLight_vec[i].position);
			pbrshader.setUniform("light[" + std::to_string(i) + "].LightColor", light.PointLight_vec[i].Color);
		}

		spheres.renderSphere(1,2,10.5 , pbrshader);


		//if imgui:: IBL scene
		//TODO:  for IBL:
		//DIFUSE HDR: IRRADIANCE CUBE MAP.	(OFFLINE RENDER)
		//SPECUALR HDR: SPLIT SUM (OFFLINE RENDER)


		//render light cubes/spheres.

		//shaodw mapping? requires different scenes i guess?

		//add post processing too! (SSAO/ bloom)



		//bagshader.use();
		//bagshader.setUniform("model", cubemodel);
		//bagshader.setUniform("view", view);
		//bagshader.setUniform("projection", projection);
		//bag.Draw(bagshader);	//set up the textures in draw call.

		//cubeshader.use();
		//cubeshader.setUniform("lightPos", lightpos);
		//cubeshader.setUniform("lightColor", glm::vec3(1.0, 0.5, 0.2));
		//cubeshader.setUniform("objectColor", glm::vec3(0.4, 0.8, 0.5));
		//cubeshader.setUniform("model", cubemodel);
		//cubeshader.setUniform("view", view);
		//cubeshader.setUniform("projection", projection);
		//CubeVAO.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		



		//render skybox last
		//use imgui to change cubemap texture.
		sky.setCubeMapTexture(envCubeMap);
		sky.DrawSkyBox(skyboxshader,cam.GetViewMat(),projection);

		
		
		t += 1.0f;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


