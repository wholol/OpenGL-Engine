#include "VertexBuffer.h"



VertexBuffer::VertexBuffer(size_t vertexsize, void* vertexdata, size_t setstridesize)
	:stride(setstridesize)
{
	glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, vertexsize, vertexdata, GL_STATIC_DRAW);
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}


void VertexBuffer::addattriblayout(int num_components, GLenum type, bool normalized)
{
	layout.push_back({ num_components , type , normalized ,offset_counter , sizeof(type)});
	offset_counter += sizeof(type);		//increment the offset
}

std::vector<vertexattriblayout>& VertexBuffer::getattriblayout()
{
	return layout;
}

size_t VertexBuffer::getstridesize() const
{
	return stride;
}
