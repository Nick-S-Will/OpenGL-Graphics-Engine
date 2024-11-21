#pragma once
#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"
#include "Texture.h"
#include "OBJ_Loader.h"

class Shader;

enum class LightType : int {
	Directional = 0,
	Point = 1,
	Spot = 2
};

class Mesh
{
public:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale = glm::vec3(1.f);
	glm::vec3 color = glm::vec3(1.f);
	glm::vec2 textureOffset;
	LightType lightType;
	bool isEnabled = true;

	Mesh() = default;
	virtual ~Mesh();

	glm::mat4 GetTransform() const { return glm::translate(glm::mat4(1), position) * GetRotationFromEulerAngles(eulerAngles) * glm::scale(glm::mat4(1), scale); }
	glm::mat4 GetRotationMatrix() const { return GetRotationFromEulerAngles(eulerAngles); }
	glm::vec3 GetRight() const { return GetRotationMatrix() * glm::vec4(1.f, 0.f, 0.f, 0.f); }
	glm::vec3 GetUp() const { return GetRotationMatrix() * glm::vec4(0.f, 1.f, 0.f, 0.f); }
	glm::vec3 GetForward() const { return GetRotationMatrix() * glm::vec4(0.f, 0.f, -1.f, 0.f); }
	std::string GetLightTypeName() const
	{
		switch (lightType) {
		case LightType::Directional: return "Directional";
		case LightType::Point: return "Point";
		case LightType::Spot: return "Spot";
		default: return "Unknown";
		}
	}
	int GetInstanceCount() const { return instanceCount; }
	bool HasNormalMapEnabled() const { return vertexStride == 14; }
	bool HasInstancingEnabled() const { return instanceCount > 1; }

	void Create(Shader* shader, std::string filePath, bool normalMapEnabled, GLenum textureWrapMode, int instanceCount = 1);
	void AddInstance() { AddInstance(true); }
	void RemoveInstance();
	void Cleanup();
	void Render(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes);

private:
	Shader* shader = nullptr;
	Texture texture = {};
	Texture specularTexture = {};
	Texture normalTexture = {};
	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;
	GLuint instanceBuffer = 0;
	std::vector<GLfloat> vertexData;
	std::vector<GLubyte> indexData;
	std::vector<GLfloat> instanceData;
	int instanceCount = 0;
	int vertexStride = 0;

	void AddInstance(bool updateBuffer);
	void UpdateInstanceBuffer();
	void BindAttributes();
	void SetShaderVariables(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes);
	
	void static CalculateTangents(std::vector<objl::Vertex> vertices, objl::Vector3& tangent, objl::Vector3& bitangent)
	{
		objl::Vector3 edge1 = vertices[1].Position - vertices[0].Position;
		objl::Vector3 edge2 = vertices[2].Position - vertices[0].Position;
		objl::Vector2 deltaUV1 = vertices[1].TextureCoordinate - vertices[0].TextureCoordinate;
		objl::Vector2 deltaUV2 = vertices[2].TextureCoordinate - vertices[0].TextureCoordinate;

		float f = 1.f / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);

		tangent.X = f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X);
		tangent.Y = f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y);
		tangent.Z = f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z);

		bitangent.X = f * (-deltaUV2.X * edge1.X + deltaUV1.X * edge2.X);
		bitangent.Y = f * (-deltaUV2.X * edge1.Y + deltaUV1.X * edge2.Y);
		bitangent.Z = f * (-deltaUV2.X * edge1.Z + deltaUV1.X * edge2.Z);
	}

	static glm::mat4 GetRotationFromEulerAngles(const glm::vec3& eulerAngles)
	{
		glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));

		return rotationZ * rotationY * rotationX;
	}

	static std::string GetFileName(std::string& filePath)
	{
		const size_t last_slash_index = filePath.find_last_of("\\/");
		if (std::string::npos != last_slash_index) filePath.erase(0, last_slash_index + 1);

		return filePath;
	}
};

#endif // !MESH_H