#include "Blur.h"
#include "GL/eglew.h"

Blur::Blur(int screenwidth, int screenheight)
	:screenheight(screenheight) , screenwidth(screenwidth)
{
	glGenFramebuffers(1, &FBO);
	glGenFramebuffers(2, pingpongFBO);
}

void Blur::Extract_Colour()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	for (int i = 0; i < 2; ++i)
	{
		ColorBuffers[i].Bind();
		ColorBuffers[i].genTexture(screenwidth, screenheight, GL_RGBA16F, GL_RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, ColorBuffers[i].getTexID(), 0);
	}

	glDrawBuffers(2, attachments);	//tell opengl that there will be 2 renderign colour buffers

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Blur::Gaussian_Blur()
{
	for (int i = 0; i < 2; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		pingpongTexture[i].Bind();
		pingpongTexture[i].genTexture(screenwidth, screenheight, GL_RGBA16F, GL_RGBA);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTexture[i].getTexID(), 0);
		
	}

	blur_shader.use();
	blur_shader.setUniform("image", 0);
	glActiveTexture(GL_TEXTURE0);
	ColorBuffers[1].Bind();	//bind the buffer textrure that has brightness filtered.

	bool horizontal = true;
	int num_times = 10;	//blur the image in 10 passes - 5 horizontal , 5 vertical.

	for (int i = 0; i < num_times; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		blur_shader.setUniform("horizontal", horizontal);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongTexture[horizontal].getTexID(), 0);
		horizontal = !horizontal;
		//RENDER QUAD HERE

		blur_shader.setUniform("image", 0);
		glActiveTexture(GL_TEXTURE0);
		pingpongTexture[!horizontal].Bind();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Blur::merge_image()
{
	//no need for a framebuffer, we want to render to defualt framebuffer
	merge_image_shader.use();
	merge_image_shader.setUniform("blurred_image", 0);
	glActiveTexture(GL_TEXTURE0);
	pingpongTexture[1].Bind();
	merge_image_shader.setUniform("non_blurred_image", 1);
	glActiveTexture(GL_TEXTURE1);
	ColorBuffers[0].Bind();
}

