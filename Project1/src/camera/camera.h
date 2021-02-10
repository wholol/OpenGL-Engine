#pragma once
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"
#include <iostream>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class camera
{
private:
	static constexpr float YAW = -90.0f;	//ensures camera ponits to -z
	static constexpr float PITCH = 0.0f;
	static constexpr float SPEED = 2.5f;
	static constexpr float SENSITIVITY = 0.1f;
	static constexpr float ZOOM = 45.0f;
public:
	camera(glm::vec3 camerapos = glm::vec3{ 0.0f,0.0f,0.0f }, glm::vec3 forward = glm::vec3{ 0.0f,0.0f,-1.0f }, float yaw = YAW, float pitch = PITCH, float camspeed = SPEED,float sensitivity = SENSITIVITY);
	void movecamera(Camera_Movement Dir , float dt);
	void rotatecamera(float xoffset, float yoffset, bool constrainpitch = true);
	glm::mat4 GetViewMat();

public:
	glm::vec3 camerapos;


private:
	glm::vec3 target;
	
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 forward;
	static constexpr glm::vec3 tmp = glm::vec3{ 0.0f , 1.0f , 0.0f };

	float camspeed;
	float sensitivity;
	float zoom;
	float yaw;
	float pitch;

};