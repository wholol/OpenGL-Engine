#pragma once
#include <vector>
#include <gl/glew.h>
#include <vector>

struct vertexattriblayout
{
	GLint num_components;
	GLenum type;
	GLboolean normalized;
	size_t offset;
	size_t individual_sizeof;	//sizeof value for indivual components
};

class VertexBuffer
{
public:
	VertexBuffer() {};
	VertexBuffer(size_t vertexsize,void* vertexdata , size_t setstridesize);	//calls genbuffers,bind,bufferdata by default
	void Bind();
	void addattriblayout(int num_components,GLenum type, bool normalized = false);
	std::vector<vertexattriblayout>& getattriblayout();
	size_t getstridesize() const;


private:
	unsigned int ID;
	std::vector<vertexattriblayout> layout;
	size_t stride;
	size_t offset_counter;
};