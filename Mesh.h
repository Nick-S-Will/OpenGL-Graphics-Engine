#pragma once
#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"
#include "Texture.h"

class Shader;

class Mesh
{
public:
	glm::vec3 position;
	glm::vec3 eulerAngles;
	glm::vec3 scale = glm::vec3(1.f);
	glm::vec2 textureOffset;

	Mesh() = default;
	virtual ~Mesh();

	glm::mat4 GetTransform() const { return glm::translate(glm::mat4(1), position) * GetRotationFromEulerAngles(eulerAngles) * glm::scale(glm::mat4(1), scale); }

	void Create(Shader* shader, GLenum textureWrapMode);
	void Cleanup();
	void Render(glm::mat4 vp, glm::vec3 lightPosition, glm::vec3 lightColor, float specularStrength, glm::vec3 cameraPosition);

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
	void SetShaderVariables(glm::mat4 vp, glm::vec3 lightPosition, glm::vec3 lightColor, float specularStrength, glm::vec3 cameraPosition);
};

#endif // !MESH_H