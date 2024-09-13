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
	void Render();

private:
	Shader* shader = nullptr;
	GLuint vertexBuffer = 0;
	std::vector<GLfloat> vertexData;
};

#endif // !MESH_H