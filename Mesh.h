#pragma once
#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"
#include "Texture.h"

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

	void Create(Shader* shader, std::string filePath, GLenum textureWrapMode);
	void Cleanup();
	void Render(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes);

private:
	Shader* shader = nullptr;
	Texture texture = {};
	Texture texture2 = {};
	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;
	std::vector<GLfloat> vertexData;
	std::vector<GLubyte> indexData;

	static glm::mat4 GetRotationFromEulerAngles(const glm::vec3& eulerAngles)
	{
		glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));

		return rotationZ * rotationY * rotationX;
	}

	void BindAttributes();
	void SetShaderVariables(glm::mat4 vp, glm::vec3 cameraPosition, std::vector<Mesh*>& lightMeshes);
};

#endif // !MESH_H