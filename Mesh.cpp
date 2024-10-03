#include "Mesh.h"
#include "Shader.h"

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create(Shader* shader, GLenum textureWrapMode)
{
	this->shader = shader;
	texture = Texture();
	texture.LoadTexture("./Assets/Textures/Tacos.jpg", textureWrapMode);
	
	texture2 = Texture();
	texture2.LoadTexture("./Assets/Textures/Pattern.png", textureWrapMode);

	float a = 50.f;
	vertexData = {
		// Position		RGB Color		UV
		a, a, 0.f,		1.f, 0.f, 0.f,	1.f, 1.f,
		a, -a, 0.f,		0.f, 1.f, 0.f,	1.f, 0.f,
		-a, -a, 0.f,	0.f, 0.f, 1.f,	0.f, 0.f,
		-a, a, 0.f,		1.f, 1.f, 1.f,	0.f, 1.f
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);

	indexData = {
		2, 0, 3, 2, 1, 0
	};
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexData.size() * sizeof(GLubyte), indexData.data(), GL_STATIC_DRAW);
}

void Mesh::Cleanup()
{
	texture.CleanUp();
	texture2.CleanUp();
	if (vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
	if (indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
	vertexBuffer = 0;
	indexBuffer = 0;
}

void Mesh::Render(glm::mat4 wvp)
{
	glUseProgram(shader->GetProgramID());

	wvp *= GetTransform();
	glUniformMatrix4fv(shader->GetWVP(), 1, FALSE, &wvp[0][0]);
	
	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	
	glEnableVertexAttribArray(shader->GetColors());
	glVertexAttribPointer(shader->GetColors(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	
	glEnableVertexAttribArray(shader->GetTextureCoords());
	glVertexAttribPointer(shader->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	glEnableVertexAttribArray(shader->GetTextureOffset());
	glUniform2fv(shader->GetTextureOffset(), 1, &textureOffset[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
	glUniform1i(shader->GetSampler1(), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2.GetTexture());
	glUniform1i(shader->GetSampler1(), 1);

	glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_BYTE, (void*)0);
	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetColors());
	glDisableVertexAttribArray(shader->GetTextureCoords());
	glDisableVertexAttribArray(shader->GetTextureOffset());
}