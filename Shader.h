#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "StandardIncludes.h"

class Shader
{
public:
	Shader() = default;
	virtual ~Shader();

	GLuint GetProgramID() { return programID; }
	GLuint GetVertices() { return vertices; }

	void LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);
	void Cleanup();
	GLuint GetAttributeWVP() { return attributeWVP; }

private:
	void CreateShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
	GLuint LoadShaderFile(const char* filePath, GLenum type);
	void LoadAttributes();
	void EvaluateShader(int infoLength, GLuint id);

	GLuint programID = 0;
	GLuint vertices = 0;
	GLuint attributeWVP = 0;
	GLint result = GL_FALSE;
	int infoLogLength = 0;
};

#endif // !SHADER_H