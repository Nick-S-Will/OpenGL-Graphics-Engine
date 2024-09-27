#pragma once
#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"
class Shader;

class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh();

	void Create(Shader* shader);
	void Cleanup();
	void Render(glm::mat4 wvp, float deltaTime);

private:
	Shader* shader = nullptr;
	GLuint vertexBuffer = 0, indexBuffer = 0;
	std::vector<GLfloat> vertexData;
	std::vector<GLubyte> indexData;
	glm::mat4 world = glm::mat4(1);
};

#endif // !MESH_H