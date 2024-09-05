#include "Mesh.h"

Mesh::Mesh()
{
	vertexBuffer = 0;
}

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create()
{
	vertexData = { -1.f, -1.f, 0.f, 1.f, -1.f, 0.f, 0.f, 1.f, 0.f };
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
}

void Mesh::Cleanup()
{
	if (vertexBuffer == 0) return;

	glDeleteBuffers(1, &vertexBuffer);
}

void Mesh::Render()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);
}
