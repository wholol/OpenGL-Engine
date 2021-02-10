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


	//light setup
	Light light;
	glm::vec3 light_color(300.0f, 300.0f, 300.0f);
	light.addPointLight({ glm::vec3(-10.0f , 10.0f , 10.0f) , light_color });
	light.addPointLight({ glm::vec3(10.0f , 10.0f , 10.0f) , light_color });
	light.addPointLight({ glm::vec3(-10.0f , -10.0f , 10.0f) , light_color });
	light.addPointLight({ glm::vec3(10.0f , -10.0f , 10.0f) , light_color });

	//sphere
	Shapes spheres;
	Shader pbrshader( "assets/shaders/pbr.vs" , "assets/shaders/pbr.fs" );


	float t = 0.0f;
	
	//Shader cubeshader("assets/shaders/cubevertex.vs", "assets/shaders/cubefragment.fs");	//for drawing ad shading cube objects
	//Shader lightcube("assets/shaders/lightvertex.vs","assets/shaders/lightfrag.fs");	//for drawing light cubes


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
		
		pbrshader.use();
		pbrshader.setUniform("view", view);
		pbrshader.setUniform("camPos", cam.camerapos);
		pbrshader.setUniform("projection", projection);
		pbrshader.setUniform("ao", 1.0f);
		pbrshader.setUniform("albedo",glm::vec3(0.5f , 0.0f , 0.0f));

		for (int i = 0; i < light.numPointLights(); ++i)
		{
			pbrshader.setUniform("light[" + std::to_string(i) + "].LightPos", light.PointLight_vec[i].position);
			pbrshader.setUniform("light[" + std::to_string(i) + "].LightColor", light.PointLight_vec[i].Color);
		}

		spheres.renderSphere(1, 2,10.5f,pbrshader);



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
		
		sky.DrawSkyBox(skyboxshader,cam.GetViewMat(),projection);

		//lightcube.use();
		//lightcube.setUniform("model", lightmodel);
		//lightcube.setUniform("view", view);
		//lightcube.setUniform("projection", projection);
		//
		//LightSrcVAO.Bind();
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		t += 1.0f;
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}




