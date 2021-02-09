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


double lastX = 400, lastY = 300;	//initialize mouse pos to center of screen
bool firstMouse = true;

float deltatime = 0.0f;
float lastframe = 0.0f;

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
	//input
	glfwSetCursorPosCallback(window, mouse_callback);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	float vertices[] = {
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f// positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 cubePositions[] = {		//cube transformation matrices
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] = {		//point light positions
	   glm::vec3(0.7f,  0.2f,  2.0f),
	   glm::vec3(2.3f, -3.3f, -4.0f),
	   glm::vec3(-4.0f,  2.0f, -12.0f),
	   glm::vec3(0.0f,  0.0f, -3.0f)
	};
	
	VertexArray CubeVAO;	//cube's VAO
	VertexBuffer CubeVBO(sizeof(vertices), vertices, 8 * sizeof(float));
	CubeVBO.addattriblayout(3, GL_FLOAT, GL_FALSE);
	CubeVBO.addattriblayout(3, GL_FLOAT, GL_FALSE);
	CubeVBO.addattriblayout(2, GL_FLOAT, GL_FALSE);
	CubeVAO.EnableAttribs(CubeVBO);

	VertexArray LightSrcVAO;		//LIGHT SOURCE VAO
	LightSrcVAO.EnableAttribs(CubeVBO, 0);

	VertexArray Lights;
	
	//skybox setup
	std::vector<std::string> skyboxjpg;
	skyboxjpg.emplace_back("right.jpg");
	skyboxjpg.emplace_back("left.jpg");
	skyboxjpg.emplace_back("top.jpg");
	skyboxjpg.emplace_back("bottom.jpg");
	skyboxjpg.emplace_back("front.jpg");
	skyboxjpg.emplace_back("back.jpg");

	CustomTexture skybox;
	skybox.loadCubeMap(skyboxjpg);
	Shader skyboxshader("assets/shaders/skyvert.vs","assets/shaders/skyfrag.fs");

	SkyBox sky(skybox);
	
	Model bag("assets/models/backpack/backpack.obj");
	Shader bagshader("assets/shaders/bagvert.vs", "assets/shaders/bagfrag.fs");

	stbi_set_flip_vertically_on_load(true);

	////load diffuse map
	//unsigned int diffusemap;
	//glGenTextures(1, &diffusemap);
	//// load and generate the texture
	//int width, height, nrComponents;
	//unsigned char *data = stbi_load("assets/textures/container.jpg", &width, &height, &nrComponents, 0);
	//
	//if (data)
	//{
	//	GLenum format;
	//	if (nrComponents == 1) format = GL_RED;
	//	else if (nrComponents == 3) format = GL_RGB;
	//	else if (nrComponents == 4) format = GL_RGBA;
	//
	//	
	//	glBindTexture(GL_TEXTURE_2D, diffusemap);
	//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//	
	//	// set the texture wrapping/filtering options (on the currently bound texture object)
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	stbi_image_free(data);
	//}
	//
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//	throw std::runtime_error("failed texture loading!");
	//	stbi_image_free(data);
	//}
	//
	////specular map
	//unsigned int specularmap;
	//glGenTextures(1, &specularmap);
	//data = stbi_load("assets/textures/container_specular.jpg", &width, &height, &nrComponents, 0);
	//if (data)
	//{
	//	GLenum format;
	//	if (nrComponents == 1) format = GL_RED;
	//	else if (nrComponents == 3) format = GL_RGB;
	//	else if (nrComponents == 4) format = GL_RGBA;
	//
	//	glBindTexture(GL_TEXTURE_2D, specularmap);
	//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//
	//	// set the texture wrapping/filtering options (on the currently bound texture object)
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	stbi_image_free(data);
	//}
	//
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//	throw std::runtime_error("failed texture loading!");
	//	stbi_image_free(data);
	//}


	float t = 0.0f;
	/*the loop*/
	Shader cubeshader("assets/shaders/cubevertex.vs", "assets/shaders/cubefragment.fs");	//for drawing ad shading cube objects
	Shader lightcube("assets/shaders/lightvertex.vs","assets/shaders/lightfrag.fs");	//for drawing light cubes




	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		float currentFrame = glfwGetTime();
		deltatime = currentFrame - lastframe;
		lastframe = currentFrame;
		processInput(window);
		
		
		glm::mat4 cubemodel = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		
		cubemodel = glm::rotate(cubemodel, glm::radians(t), glm::vec3(1.0f, 0.0f, 0.0f));
		view = cam.GetViewMat();
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		

		bagshader.use();
		bagshader.setUniform("model", cubemodel);
		bagshader.setUniform("view", view);
		bagshader.setUniform("projection", projection);
		bag.Draw(bagshader);	//set up the textures in draw call.

		//cubeshader.use();
		//cubeshader.setUniform("lightPos", lightpos);
		//cubeshader.setUniform("lightColor", glm::vec3(1.0, 0.5, 0.2));
		//cubeshader.setUniform("objectColor", glm::vec3(0.4, 0.8, 0.5));
		//cubeshader.setUniform("model", cubemodel);
		//cubeshader.setUniform("view", view);
		//cubeshader.setUniform("projection", projection);
		//CubeVAO.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//skybox
		//glDepthFunc(GL_LEQUAL);
		//skyboxshader.use();
		//view = glm::mat4(glm::mat3(cam.GetViewMat()));	//remove translation to prevent skybox moving
		//skyboxshader.setUniform("view", view);
		//skyboxshader.setUniform("projection", projection);
		//SkyBoxVAO.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glActiveTexture(GL_TEXTURE0);
		//skybox.Bind();
		//glDepthFunc(GL_LESS);
		
		sky.DrawSkyBox(skyboxshader,cam.GetViewMat(),projection);

		//
		//lightcube.use();
		//lightcube.setUniform("model", lightmodel);
		//lightcube.setUniform("view", view);
		//lightcube.setUniform("projection", projection);
		//
		//LightSrcVAO.Bind();
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//
		t += 1.0f;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}




