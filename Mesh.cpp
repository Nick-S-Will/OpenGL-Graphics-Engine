#include "Mesh.h"
#include "Shader.h"
#include <OBJ_Loader.h>

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create(Shader* shader, std::string filePath, GLenum textureWrapMode)
{
	this->shader = shader;

	objl::Loader loader;
	M_ASSERT(loader.LoadFile(filePath), "Failed to load mesh");

	for (int i = 0; i < loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh mesh = loader.LoadedMeshes[i];
		for (int j = 0; j < mesh.Vertices.size(); j++)
		{
			vertexData.push_back(mesh.Vertices[j].Position.X);
			vertexData.push_back(mesh.Vertices[j].Position.Y);
			vertexData.push_back(mesh.Vertices[j].Position.Z);
			vertexData.push_back(mesh.Vertices[j].Normal.X);
			vertexData.push_back(mesh.Vertices[j].Normal.Y);
			vertexData.push_back(mesh.Vertices[j].Normal.Z);
			vertexData.push_back(mesh.Vertices[j].TextureCoordinate.X);
			vertexData.push_back(mesh.Vertices[j].TextureCoordinate.Y);
		}
	}

	std::string textureName = loader.LoadedMaterials[0].map_Kd;
	auto lastSlashIndex = textureName.find_last_of("\\/");
	if (lastSlashIndex != std::string::npos) textureName.erase(0, lastSlashIndex + 1);
	
	texture = Texture();
	texture.LoadTexture("./Assets/Textures/" + textureName, textureWrapMode);

	texture2 = Texture();
	texture2.LoadTexture("./Assets/Textures/" + textureName, textureWrapMode);

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
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(shader->GetNormals());
	glVertexAttribPointer(shader->GetNormals(), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(shader->GetTextureCoords());
	glVertexAttribPointer(shader->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

void Mesh::SetShaderVariables(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes, glm::vec3 lightColor)
{
	shader->SetMat4("World", GetTransform());
	shader->SetMat4("WVP", vp * GetTransform());
	shader->SetVec3("CameraPosition", cameraPosition);

	shader->SetFloat("material.specularStrength", 8);
	shader->SetTextureSampler("material.diffuseTexture", GL_TEXTURE0, 0, texture.GetTexture());
	shader->SetTextureSampler("material.specularTexture", GL_TEXTURE1, 1, texture2.GetTexture());

	for (int i = 0; i < lightMeshes.size(); i++)
	{
		std::string arrayName = "lights";
		shader->SetArrayVec3(arrayName, i, "position", lightMeshes[i]->position);
		shader->SetArrayVec3(arrayName, i, "direction", lightMeshes[i]->GetForward());
		shader->SetArrayVec3(arrayName, i, "color", lightColor);
		shader->SetArrayVec3(arrayName, i, "diffuseColor", { 1.0f, 1.0f, 1.0f });
		shader->SetArrayVec3(arrayName, i, "specularColor", { 3.0f, 3.0f, 3.0f });
		shader->SetArrayFloat(arrayName, i, "constant", 1.f);
		shader->SetArrayFloat(arrayName, i, "linear", .09f);
		shader->SetArrayFloat(arrayName, i, "quadratic", .032f);
		shader->SetArrayFloat(arrayName, i, "coneAngle", glm::radians(15.f));
		shader->SetArrayFloat(arrayName, i, "persistence", 30.f);
		shader->SetArrayInt(arrayName, i, "isPointLight", 1);
	}

	shader->SetVec3("ambientColor", glm::vec3(.2f));
}

void Mesh::Render(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes, glm::vec3 lightColor)
{
	glUseProgram(shader->GetProgramID());

	BindAttributes();
	SetShaderVariables(vp, cameraPosition, lightMeshes, lightColor);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 8);
	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetNormals());
	glDisableVertexAttribArray(shader->GetTextureCoords());
}