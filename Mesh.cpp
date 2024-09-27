#include "Mesh.h"
#include "Shader.h"

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create(Shader* shader)
{
	this->shader = shader;

	float a = 26.0f;
	float b = 42.0f;
	vertexData = {
		// Position		RGBA Color
		-a, 0.0f, b,	1.0f,	0.0f,	0.0f,	1.0f,
		a, 0.0f, b,		1.0f,	0.549f,	0.0f,	1.0f,
		-a, 0.0f, -b,	1.0f,	1.0f,	0.0f,	1.0f,
		a, 0.0f, -b,	1.0f,	1.0f,	0.0f,	1.0f,
		0.0f, b, a,		0.0f,	0.0f,	1.0f,	1.0f,
		0.0f, b, -a,	0.294f,	0.0f,	0.51f,	1.0f,
		0.0f, -b, a,	0.502f,	0.0f,	0.502f,	1.0f,
		0.0f, -b, -a,	1.0f,	1.0f,	1.0f,	1.0f,
		b, a, 0.0f,		0.0f,	1.0f,	1.0f,	1.0f,
		-b, a, 0.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		b, -a, 0.0f,	0.118f,	0.565f,	1.0f,	1.0f,
		-b, -a, 0.0f,	0.863f,	0.078f,	0.235f,	1.0f
	};
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

	indexData = {
		0, 6, 1, 0, 11, 6, 1, 4, 0, 1, 8, 4,
		1, 10, 8, 2, 5, 3, 2, 9, 5, 2, 11, 9,
		3, 7, 2, 3, 10, 7, 4, 8, 5, 4, 9, 0,
		5, 8, 3, 5, 9, 4, 6, 10, 1, 6, 11, 7,
		7, 10, 6, 7, 11, 2, 8, 10, 3, 9, 11, 0
	};
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexData.size() * sizeof(GLubyte), indexData.data(), GL_STATIC_DRAW);
}

void Mesh::Cleanup()
{
	if (vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
	if (indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
	vertexBuffer = 0;
	indexBuffer = 0;
}

void Mesh::Render(glm::mat4 wvp, glm::vec3 eulerAngles, float scale)
{
	glUseProgram(shader->GetProgramID());

	wvp *= glm::scale(glm::mat4(1), scale * glm::vec3(1)) * GetRotationFromEulerAngles(eulerAngles) * world;
	glUniformMatrix4fv(shader->GetAttributeWVP(), 1, FALSE, &wvp[0][0]);
	
	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(shader->GetColors());
	glVertexAttribPointer(shader->GetColors(), 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_BYTE, (void*)0);
	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetColors());
}