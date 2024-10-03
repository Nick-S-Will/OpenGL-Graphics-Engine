#pragma once
#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"
#include "Texture.h"

class Shader;

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh();

	void Create(Shader* shader, GLenum textureWrapMode);
	void Cleanup();
	void Render(glm::mat4 wvp, glm::vec3 eulerAngles, float scale);

private:
	Shader* shader = nullptr;
	Texture texture {};
	GLuint vertexBuffer = 0, indexBuffer = 0;
	std::vector<GLfloat> vertexData;
	std::vector<GLubyte> indexData;
	glm::mat4 world = glm::mat4(1);
	glm::vec3 position;
	glm::vec3 rotation;

	static glm::mat4 GetRotationFromEulerAngles(const glm::vec3& eulerAngles)
	{
		glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), eulerAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), eulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), eulerAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));

		return rotationZ * rotationY * rotationX;
	}
};

#endif // !MESH_H