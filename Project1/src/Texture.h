#pragma once
#include <string>
#include <gl/glew.h>
#include <vector>
#include "stb_image.h"
#include <iostream>
#include <string>

class Texture
{
public:
	Texture(); 
	void loadCubeMap(const std::vector<std::string>& img_names);
	void loadHDR(const std::string& img_name);
	void loadTexture(const std::string& img_name);
	void Bind();

private:
	std::string filepath = "assets/textures/";
	unsigned int ID;
};