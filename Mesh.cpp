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

	vertexData = {
		// Position				Normals				Texture Coords
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
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

void Mesh::BindAttributes()
{
	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(shader->GetNormals());
	glVertexAttribPointer(shader->GetNormals(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(shader->GetTextureCoords());
	glVertexAttribPointer(shader->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
	glUniform1i(shader->GetSampler1(), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2.GetTexture());
	glUniform1i(shader->GetSampler2(), 1);
}

void Mesh::SetShaderVariables(glm::mat4 vp, glm::vec3 lightPosition, glm::vec3 lightColor, float specularStrength, glm::vec3 cameraPosition)
{
	shader->SetMat4("World", GetTransform());
	shader->SetMat4("WVP", vp * GetTransform());
	shader->SetVec3("CameraPosition", cameraPosition);

	shader->SetFloat("SpecularStrength", specularStrength);
	shader->SetVec3("SpecularColor", { 3.0f, 3.0f, 3.0f });
	shader->SetVec3("LightDirection", glm::normalize(lightPosition - position));
	shader->SetVec3("LightColor", lightColor);
	shader->SetVec3("AmbientLight", { 0.1f, 0.1f, 0.1f });
	shader->SetVec3("DiffuseColor", { 1.0f, 1.0f, 1.0f });
}

void Mesh::Render(glm::mat4 vp, glm::vec3 lightPosition, glm::vec3 lightColor, float specularStrength, glm::vec3 cameraPosition)
{
	glUseProgram(shader->GetProgramID());

	BindAttributes();
	SetShaderVariables(vp, lightPosition, lightColor, specularStrength, cameraPosition);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 8);
	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetNormals());
	glDisableVertexAttribArray(shader->GetTextureCoords());
}