#include "Mesh.h"
#include "Shader.h"

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create(Shader* shader, std::string filePath, bool normalMapEnabled, GLenum textureWrapMode)
{
	this->shader = shader;

	objl::Loader loader;
	M_ASSERT(loader.LoadFile(filePath), "Failed to load mesh");

	for (int i = 0; i < loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh mesh = loader.LoadedMeshes[i];
		std::vector<objl::Vector3> tangents;
		std::vector<objl::Vector3> bitangents;
		std::vector<objl::Vertex> triangle;
		objl::Vector3 tangent;
		objl::Vector3 bitangent;
		if (normalMapEnabled)
		{
			for (int j = 0; j < mesh.Vertices.size(); j += 3)
			{
				triangle.clear();
				triangle.push_back(mesh.Vertices[j]);
				triangle.push_back(mesh.Vertices[j + 1]);
				triangle.push_back(mesh.Vertices[j + 2]);
				CalculateTangents(triangle, tangent, bitangent);
				tangents.push_back(tangent);
				bitangents.push_back(bitangent);
			}
		}

		for (int j = 0; j < mesh.Vertices.size(); j++)
		{
			vertexData.push_back(mesh.Vertices[j].Position.X);
			vertexData.push_back(mesh.Vertices[j].Position.Y);
			vertexData.push_back(mesh.Vertices[j].Position.Z);
			vertexData.push_back(mesh.Vertices[j].Normal.X);
			vertexData.push_back(mesh.Vertices[j].Normal.Y);
			vertexData.push_back(mesh.Vertices[j].Normal.Z);

			if (normalMapEnabled && loader.LoadedMaterials[0].map_bump != "")
			{
				int index = j / 3;
				vertexData.push_back(tangents[index].X);
				vertexData.push_back(tangents[index].Y);
				vertexData.push_back(tangents[index].Z);
				vertexData.push_back(bitangents[index].X);
				vertexData.push_back(bitangents[index].Y);
				vertexData.push_back(bitangents[index].Z);
			}

			vertexData.push_back(mesh.Vertices[j].TextureCoordinate.X);
			vertexData.push_back(mesh.Vertices[j].TextureCoordinate.Y);
		}
	}

	vertexStride = normalMapEnabled ? 14 : 8;

	std::string textureDirectory = "./Assets/Textures/";
	std::string texturePath = loader.LoadedMaterials[0].map_Kd;
	std::string textureFileName = texturePath != "" ? GetFileName(texturePath) : "Tacos.jpg";
	texture = Texture();
	texture.LoadTexture(textureDirectory + textureFileName, textureWrapMode);

	texturePath = loader.LoadedMaterials[0].map_Ks;
	textureFileName = texturePath != "" ? GetFileName(texturePath) : "Pattern.png";
	specularTexture = Texture();
	specularTexture.LoadTexture(textureDirectory + textureFileName, textureWrapMode);

	texturePath = loader.LoadedMaterials[0].map_bump;
	textureFileName = texturePath != "" ? GetFileName(texturePath) : "BrickWallNormal.jpg";
	normalTexture = Texture();
	normalTexture.LoadTexture(textureDirectory + textureFileName, textureWrapMode);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
}

void Mesh::Cleanup()
{
	texture.CleanUp();
	specularTexture.CleanUp();
	if (vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
	if (indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
	vertexBuffer = 0;
	indexBuffer = 0;
}

void Mesh::Render(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes)
{
	if (!isEnabled) return;

	glUseProgram(shader->GetProgramID());

	BindAttributes();
	SetShaderVariables(vp, cameraPosition, lightMeshes);

	glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / vertexStride);
	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetNormals());
	glDisableVertexAttribArray(shader->GetTextureCoords());
}

void Mesh::BindAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	int pointerIndex = 0;
	glEnableVertexAttribArray(shader->GetVertices());
	glVertexAttribPointer(shader->GetVertices(), 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)(pointerIndex * sizeof(float)));
	pointerIndex += 3;

	glEnableVertexAttribArray(shader->GetNormals());
	glVertexAttribPointer(shader->GetNormals(), 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)(pointerIndex * sizeof(float)));
	pointerIndex += 3;

	if (HasNormalMapEnabled()) {
		glEnableVertexAttribArray(shader->GetTangents());
		glVertexAttribPointer(shader->GetTangents(), 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)(pointerIndex * sizeof(float)));
		pointerIndex += 3;

		glEnableVertexAttribArray(shader->GetBitangents());
		glVertexAttribPointer(shader->GetBitangents(), 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)(pointerIndex * sizeof(float)));
		pointerIndex += 3;
	}

	glEnableVertexAttribArray(shader->GetTextureCoords());
	glVertexAttribPointer(shader->GetTextureCoords(), 2, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void*)(pointerIndex * sizeof(float)));
}

void Mesh::SetShaderVariables(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes)
{
	shader->SetMat4("World", GetTransform());
	shader->SetMat4("WVP", vp * GetTransform());
	shader->SetVec3("CameraPosition", cameraPosition);
	shader->SetInt("NormalMapsEnabled", HasNormalMapEnabled());

	shader->SetVec3("ambientColor", glm::vec3(.2f));
	shader->SetFloat("material.specularStrength", 100);
	shader->SetTextureSampler("material.diffuseTexture", GL_TEXTURE0, 0, texture.GetTexture());
	shader->SetTextureSampler("material.specularTexture", GL_TEXTURE1, 1, specularTexture.GetTexture());
	shader->SetTextureSampler("material.normalTexture", GL_TEXTURE2, 2, normalTexture.GetTexture());

	for (int i = 0; i < lightMeshes.size(); i++)
	{
		std::string arrayName = "lights";
		shader->SetArrayVec3(arrayName, i, "position", lightMeshes[i]->position);
		shader->SetArrayVec3(arrayName, i, "direction", lightMeshes[i]->GetForward());
		shader->SetArrayVec3(arrayName, i, "color", lightMeshes[i]->color);
		shader->SetArrayVec3(arrayName, i, "diffuseColor", { 1.0f, 1.0f, 1.0f });
		shader->SetArrayVec3(arrayName, i, "specularColor", { 3.0f, 3.0f, 3.0f });
		shader->SetArrayFloat(arrayName, i, "constant", 1.f);
		shader->SetArrayFloat(arrayName, i, "linear", .09f);
		shader->SetArrayFloat(arrayName, i, "quadratic", .032f);
		shader->SetArrayFloat(arrayName, i, "coneAngle", glm::radians(30.f));
		shader->SetArrayFloat(arrayName, i, "persistence", 30.f);
		shader->SetArrayInt(arrayName, i, "type", static_cast<int>(lightMeshes[i]->lightType));
		shader->SetArrayInt(arrayName, i, "isEnabled", lightMeshes[i]->isEnabled ? 1 : 0);
	}
}