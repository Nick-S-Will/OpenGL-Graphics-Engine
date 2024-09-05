#pragma once
#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	void Create();
	void Cleanup();
	void Render();

private:
	GLuint vertexBuffer;
	std::vector<GLfloat> vertexData;
};

#endif // !MESH_H