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
#include "IBL_Setup.h"

double lastX = 400, lastY = 300;	//initialize mouse pos to center of screen
bool firstMouse = true;

float deltatime = 0.0f;
float lastframe = 0.0f;

void renderCube();
void renderQuad();

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
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);














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
	Shader pbrIBLshader("assets/shaders/pbr_IBL.vs", "assets/shaders/pbr_IBL.fs");
	IBL_Setup newport_loft("newport_loft.hdr");


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
			pbrshader.setUniform("ew", ao);
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

		pbrIBLshader.use();
		pbrIBLshader.setUniform("view", view);
		pbrIBLshader.setUniform("camPos", cam.camerapos);
		pbrIBLshader.setUniform("projection", projection);
		pbrIBLshader.setUniform("ao", ao);
		pbrIBLshader.setUniform("albedo", albedo);
		pbrIBLshader.setUniform("metallic", metallic);
		pbrIBLshader.setUniform("roughness", roughness);
		//pbrIBLshader.setUniform("isTextured", isTextured);
		
		//diffuse and speuclat texture map for IBL
		pbrIBLshader.setUniform("irradianceMap", 0);
		glActiveTexture(GL_TEXTURE0);
		newport_loft.diffuse_irradiance_Bind();
		pbrIBLshader.setUniform("prefilterMap", 1);
		glActiveTexture(GL_TEXTURE1);
		newport_loft.specular_cubemap_Bind();
		pbrIBLshader.setUniform("brdfLUT", 2);
		glActiveTexture(GL_TEXTURE2);
		newport_loft.BRDF_intergration_map_Bind();
		//
		//
		//
		for (int i = 0; i < light.numPointLights(); ++i)
		{
			pbrIBLshader.setUniform("light[" + std::to_string(i) + "].LightPos", light.PointLight_vec[i].position);
			pbrIBLshader.setUniform("light[" + std::to_string(i) + "].LightColor", light.PointLight_vec[i].Color);
		}

		spheres.renderSphere(1,2,10.5 , pbrIBLshader);


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
		sky.setCubeMapTexture(newport_loft.getRenderCubeMap());
		sky.DrawSkyBox(skyboxshader, cam.GetViewMat(), projection);

		
		
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

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

