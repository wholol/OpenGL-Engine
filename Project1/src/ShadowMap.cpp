#include "ShadowMap.h"
#include "GL/eglew.h"

ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &FBO);
	
	DepthMap.genTexture()

}
