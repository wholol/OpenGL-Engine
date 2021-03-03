#pragma once
#include "Texture.h"
#include "Shader.h"

class Blur
{
public:
	Blur(int screenwidth,int screenheight);
	void Extract_Colour();
	void Gaussian_Blur();
	void merge_image();


private:

	
	int screenwidth, screenheight;
	

	//bright color extraction FBO
	unsigned int FBO;
	Texture ColorBuffers[2];
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	Shader Bloom_lightsrc_shader = Shader("assets/shaders/PostProcess/bloom.vs", "assets/shaders/PostProcess/bloom_lightsource.fs");


	//gaussian blur
	unsigned int pingpongFBO[2];
	Texture pingpongTexture[2];
	Shader blur_shader = Shader("assets/shaders/PostProcess/blur.vs", "assets/shaders/PostProcess/blur.fs");

	//merge image
	Shader merge_image_shader = Shader("assets/shaders/PostProcess/blur_final.vs", "assets/shaders/PostProcess/blur_final.fs");

};