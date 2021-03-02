#pragma once
#include "Texture.h"

class ShadowMap
{
public:
	ShadowMap();

private:

	unsigned int FBO;
	
	Texture DepthMap;
	static constexpr int shadow_width = 1024;
	static constexpr int shadow_height = 1024;

};