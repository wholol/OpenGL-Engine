#pragma once
#include "VertexBuffer.h"

class VertexArray
{
public:
	VertexArray();	//calls genarray and bindarray by default
	void EnableAttribs(VertexBuffer& vb);		//should take in vertex buffer
	void Bind();
	void EnableAttribs(VertexBuffer &vb, unsigned int index);	//only enable certain attributes. (e.g. sharing dimension sizes)


private:
	unsigned int ID;
};