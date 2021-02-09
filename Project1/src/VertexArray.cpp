#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &ID);
	Bind();
}

void VertexArray::EnableAttribs(VertexBuffer &vb)
{
	const auto& layout = vb.getattriblayout();
	const auto& stride = vb.getstridesize();
	Bind();
	vb.Bind();

	for (int i = 0; i < layout.size(); ++i)
	{
		glVertexAttribPointer(i, layout[i].num_components, layout[i].type, layout[i].normalized, stride, (void*)layout[i].offset);
		glEnableVertexAttribArray(i);
	}
}

void VertexArray::Bind()
{
	glBindVertexArray(ID);
}

void VertexArray::EnableAttribs(VertexBuffer &vb,unsigned int index)
{
	const auto& layout = vb.getattriblayout();
	const auto& stride = vb.getstridesize();
	Bind();
	vb.Bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glVertexAttribPointer(index, layout[index].size, layout[index].type, layout[index].normalized, stride, (void*)layout[index].offset);
	glEnableVertexAttribArray(index);
}
