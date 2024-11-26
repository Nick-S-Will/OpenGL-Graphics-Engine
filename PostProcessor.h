#pragma once
#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "StandardIncludes.h"

class Shader;

class PostProcessor
{
public:
	void Create(Shader* shader);
	void Cleanup();
	void Start();
	void End();

private:
	Shader* shader;
	GLuint frameBufferLocation = 0;
	GLuint textureColorBufferLocation = 0;
	GLuint renderBufferObjectLocation = 0;
	GLuint vertexBufferLocation = 0;

	void CreateBuffers();
	void CreateVertices();
	void BindVertices();
};

#endif // !POST_PROCESSOR_H