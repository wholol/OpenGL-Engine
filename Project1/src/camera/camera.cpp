#include "camera.h"


camera::camera(glm::vec3 camerapos,glm::vec3 forward, float yaw, float pitch , float camspeed , float sensitivity)
	:camerapos(camerapos), yaw(yaw) , pitch(pitch) , camspeed(camspeed), sensitivity(sensitivity)
{
	
	forward.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
	forward = glm::normalize(forward);
	this->forward = forward;	//custom forard vector
	right = glm::normalize(glm::cross(forward, tmp));
	up = glm::normalize(glm::cross(right, forward));
}

void camera::movecamera(Camera_Movement Dir, float dt)
{
	float dv = camspeed * dt;

	if (Dir == FORWARD)
	{
		camerapos += forward * dv;
	}

	if (Dir == BACKWARD)
	{
		camerapos -= forward * dv;
	}

	if (Dir == LEFT)
	{
		camerapos -= right * dv;
	}

	if (Dir == RIGHT)
	{
		camerapos += right * dv;
	}
}

void camera::rotatecamera(float xoffset, float yoffset, bool constrainpitch)
{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainpitch) {
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	//update the camera vectors
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(front);
	right = glm::normalize(glm::cross(forward, tmp));
	up = glm::normalize(glm::cross(right, forward));
}

glm::mat4 camera::GetViewMat()
{
	return glm::lookAt(camerapos,camerapos + forward, up);
}
