#include "Mesh.h"
#include "Shader.h"
#include "ASEReader.h"

Mesh::~Mesh()
{
	Cleanup();
}

void Mesh::Create(Shader* shader, std::string filePath, int instanceCount)
{
	this->shader = shader;

	if (Texture::EndsWith(filePath, "ase")) LoadASE(filePath);
	else LoadOBJ(filePath);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (instanceCount <= 1)
	{
		this->instanceCount = 1;
		return;
	}

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	srand((int)glfwGetTime());
	AddInstances(instanceCount);

	UpdateInstanceBuffer();
}

void Mesh::LoadOBJ(std::string& file)
{
	objl::Loader loader;
	M_ASSERT(loader.LoadFile(file), "Failed to load mesh");

	for (int i = 0; i < (int)loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh mesh = loader.LoadedMeshes[i];
		std::vector<objl::Vector3> tangents;
		std::vector<objl::Vector3> bitangents;
		std::vector<objl::Vertex> triangle;
		objl::Vector3 tangent;
		objl::Vector3 bitangent;
		for (int j = 0; j < (int)mesh.Vertices.size(); j += 3)
		{
			triangle.clear();
			triangle.push_back(mesh.Vertices[j]);
			triangle.push_back(mesh.Vertices[j + 1]);
			triangle.push_back(mesh.Vertices[j + 2]);
			CalculateTangents(triangle, tangent, bitangent);
			tangents.push_back(tangent);
			bitangents.push_back(bitangent);
		}

		for (int j = 0; j < (int)mesh.Vertices.size(); j++)
		{
			vertexData.push_back(mesh.Vertices[j].Position.X);
			vertexData.push_back(mesh.Vertices[j].Position.Y);
			vertexData.push_back(mesh.Vertices[j].Position.Z);
			vertexData.push_back(mesh.Vertices[j].Normal.X);
			vertexData.push_back(mesh.Vertices[j].Normal.Y);
			vertexData.push_back(mesh.Vertices[j].Normal.Z);

			if (loader.LoadedMaterials[0].map_bump != "")
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

	std::string textureDirectory = "./Assets/Textures/";
	std::string texturePath = loader.LoadedMaterials[0].map_Kd;
	std::string textureFileName = texturePath != "" ? GetFileName(texturePath) : "Tacos.jpg";
	texture = Texture();
	texture.LoadTexture(textureDirectory + textureFileName, GL_REPEAT);

	texturePath = loader.LoadedMaterials[0].map_Ks;
	textureFileName = texturePath != "" ? GetFileName(texturePath) : "Pattern.png";
	specularTexture = Texture();
	specularTexture.LoadTexture(textureDirectory + textureFileName, GL_REPEAT);

	texturePath = loader.LoadedMaterials[0].map_bump;
	textureFileName = texturePath != "" ? GetFileName(texturePath) : "BrickWallNormal.jpg";
	normalTexture = Texture();
	normalTexture.LoadTexture(textureDirectory + textureFileName, GL_REPEAT);

	vertexStride = loader.LoadedMaterials[0].map_bump != "" ? 14 : 8;
}

void Mesh::LoadASE(std::string& file)
{
	ASEReader reader;
	reader.ParseASEFile(file.c_str());
	ASEReader::MeshInfo& meshInfo = reader.GeoObjects[0]->MeshInfo;
	ASEReader::Material* material = reader.Materials[reader.GeoObjects[0]->MaterialID];

	std::vector<objl::Vector3> tangents;
	std::vector<objl::Vector3> bitangents;
	std::vector<objl::Vertex> triangle;
	objl::Vector3 tangent;
	objl::Vector3 bitangent;
	int vertexCount = 0;
	for (int i = 0; i < meshInfo.FaceCount; i++)
	{
		glm::vec3 face = meshInfo.Faces[i];
		glm::vec3 textureFace = meshInfo.TextureFaces[i];
		triangle.clear();

		objl::Vertex vertex = objl::Vertex();
		glm::vec3 vector = meshInfo.Vertices[(int)face.x];
		vertex.Position = objl::Vector3(vector.x, vector.y, vector.z);
		glm::vec3 normal = meshInfo.Normals[vertexCount];
		vertex.Normal = objl::Vector3(normal.x, normal.y, normal.z);
		glm::vec2 textureCoordinate = meshInfo.TextureVertices[(int)textureFace.x];
		vertex.TextureCoordinate = objl::Vector2(textureCoordinate.x, textureCoordinate.y);
		triangle.push_back(vertex);
		vertexCount++;

		vertex = objl::Vertex();
		vector = meshInfo.Vertices[(int)face.y];
		vertex.Position = objl::Vector3(vector.x, vector.y, vector.z);
		normal = meshInfo.Normals[vertexCount];
		vertex.Normal = objl::Vector3(normal.x, normal.y, normal.z);
		textureCoordinate = meshInfo.TextureVertices[(int)textureFace.y];
		vertex.TextureCoordinate = objl::Vector2(textureCoordinate.x, textureCoordinate.y);
		triangle.push_back(vertex);
		vertexCount++;

		vertex = objl::Vertex();
		vector = meshInfo.Vertices[(int)face.z];
		vertex.Position = objl::Vector3(vector.x, vector.y, vector.z);
		normal = meshInfo.Normals[vertexCount];
		vertex.Normal = objl::Vector3(normal.x, normal.y, normal.z);
		textureCoordinate = meshInfo.TextureVertices[(int)textureFace.z];
		vertex.TextureCoordinate = objl::Vector2(textureCoordinate.x, textureCoordinate.y);
		triangle.push_back(vertex);
		vertexCount++;

		CalculateTangents(triangle, tangent, bitangent);
		tangents.push_back(tangent);
		bitangents.push_back(bitangent);

		for (int j = 0; j < 3; j++)
		{
			vertexData.push_back(triangle[j].Position.X);
			vertexData.push_back(triangle[j].Position.Y);
			vertexData.push_back(triangle[j].Position.Z);
			vertexData.push_back(triangle[j].Normal.X);
			vertexData.push_back(triangle[j].Normal.Y);
			vertexData.push_back(triangle[j].Normal.Z);

			int index = vertexCount / 3 - 1;
			vertexData.push_back(tangents[index].X);
			vertexData.push_back(tangents[index].Y);
			vertexData.push_back(tangents[index].Z);
			vertexData.push_back(bitangents[index].X);
			vertexData.push_back(bitangents[index].Y);
			vertexData.push_back(bitangents[index].Z);

			vertexData.push_back(triangle[j].TextureCoordinate.X);
			vertexData.push_back(triangle[j].TextureCoordinate.Y);
		}
	}

	std::string textureDirectory = "./Assets/Textures/";
	texture = Texture();
	if (material->Maps[0].Name == "DIFFUSE")
	{
		texture.LoadTexture(textureDirectory + GetFileName(material->Maps[0].TextureFileName), GL_REPEAT);
	}
	specularTexture = Texture();
	if (material->Maps[1].Name == "SPECULAR")
	{
		specularTexture.LoadTexture(textureDirectory + GetFileName(material->Maps[1].TextureFileName), GL_REPEAT);
	}
	normalTexture = Texture();
	if (material->Maps[1].Name == "BUMP")
	{
		normalTexture.LoadTexture(textureDirectory + GetFileName(material->Maps[1].TextureFileName), GL_REPEAT);
		vertexStride = 14;
	}
	else if (material->Maps[2].Name == "BUMP")
	{
		normalTexture.LoadTexture(textureDirectory + GetFileName(material->Maps[2].TextureFileName), GL_REPEAT);
		vertexStride = 14;
	}
	else vertexStride = 8;
}

void Mesh::AddInstances(int count)
{
	if (count < 1) return;

	for (int i = 0; i < count; i++)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.f), GetRandomVec3(50.f)) * GetRotationFromEulerAngles(GetRandomVec3(180.f));
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				instanceData.push_back(model[x][y]);
			}
		}
	}

	instanceCount += count;

	UpdateInstanceBuffer();
}

void Mesh::RemoveInstances(int count)
{
	if (count < 1 || count >= instanceCount) return;

	for (int x = 0; x < 16 * count; x++) instanceData.pop_back();

	instanceCount -= count;

	UpdateInstanceBuffer();
}

void Mesh::UpdateInstanceBuffer()
{
	glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4), instanceData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Cleanup()
{
	texture.CleanUp();
	specularTexture.CleanUp();
	normalTexture.CleanUp();
	if (vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
	if (indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
	if (instanceBuffer != 0) glDeleteBuffers(1, &instanceBuffer);
	vertexBuffer = 0;
	indexBuffer = 0;
	instanceBuffer = 0;
}

void Mesh::Render(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes, float specularStrength, glm::vec3 specularColor)
{
	if (!isEnabled) return;

	glUseProgram(shader->GetProgramID());

	BindAttributes();
	SetShaderVariables(vp, cameraPosition, lightMeshes, specularStrength, specularColor);

	if (HasInstancingEnabled())
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertexData.size() / vertexStride, instanceCount);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / vertexStride);
	}

	glDisableVertexAttribArray(shader->GetVertices());
	glDisableVertexAttribArray(shader->GetNormals());
	if (HasNormalMapEnabled())
	{
		glDisableVertexAttribArray(shader->GetTangents());
		glDisableVertexAttribArray(shader->GetBitangents());
	}
	glDisableVertexAttribArray(shader->GetTextureCoords());
	if (HasInstancingEnabled())
	{
		for (int i = 0; i < 4; i++)
		{
			glDisableVertexAttribArray(shader->GetInstanceMatrix() + i);
		}
	}
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

	if (!HasInstancingEnabled()) return;

	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(shader->GetInstanceMatrix() + i);
		glVertexAttribPointer(shader->GetInstanceMatrix() + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
	}

	for (int i = 0; i < 4; i++)
	{
		glVertexAttribDivisor(shader->GetInstanceMatrix() + i, 1);
	}
}

void Mesh::SetShaderVariables(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes, float specularStrength, glm::vec3 specularColor)
{
	shader->SetMat4("World", GetTransform());
	shader->SetMat4("WVP", vp * GetTransform());
	shader->SetVec3("CameraPosition", cameraPosition);
	shader->SetInt("NormalMapsEnabled", HasNormalMapEnabled());
	shader->SetInt("InstancingEnabled", HasInstancingEnabled());

	shader->SetVec3("ambientColor", glm::vec3(.2f));
	shader->SetFloat("material.specularStrength", specularStrength);
	shader->SetTextureSampler("material.diffuseTexture", GL_TEXTURE0, 0, texture.GetTexture());
	shader->SetTextureSampler("material.specularTexture", GL_TEXTURE1, 1, specularTexture.GetTexture());
	shader->SetTextureSampler("material.normalTexture", GL_TEXTURE2, 2, normalTexture.GetTexture());

	std::string arrayName = "lights";
	for (int i = 0; i < (int)lightMeshes.size(); i++)
	{
		shader->SetArrayVec3(arrayName, i, "position", lightMeshes[i]->position);
		shader->SetArrayVec3(arrayName, i, "direction", lightMeshes[i]->GetForward());
		shader->SetArrayVec3(arrayName, i, "color", lightMeshes[i]->color);
		shader->SetArrayVec3(arrayName, i, "diffuseColor", { 1.0f, 1.0f, 1.0f });
		shader->SetArrayVec3(arrayName, i, "specularColor", specularColor);
		shader->SetArrayFloat(arrayName, i, "constant", 1.f);
		shader->SetArrayFloat(arrayName, i, "linear", .09f);
		shader->SetArrayFloat(arrayName, i, "quadratic", .032f);
		shader->SetArrayFloat(arrayName, i, "coneAngle", glm::radians(30.f));
		shader->SetArrayFloat(arrayName, i, "persistence", 30.f);
		shader->SetArrayInt(arrayName, i, "type", static_cast<int>(lightMeshes[i]->lightType));
		shader->SetArrayInt(arrayName, i, "isEnabled", lightMeshes[i]->isEnabled);
	}
}