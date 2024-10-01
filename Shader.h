#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "StandardIncludes.h"

class Shader
{
public:
	Shader() = default;
	virtual ~Shader();

	GLuint GetProgramID() const { return programID; }
	GLuint GetVertices() const { return verticesLocation; }
	GLuint GetColors() const { return colorsLocation; }
	GLuint GetTextureCoords() { return textureCoordsLocation; }
	GLuint GetSampler1() { return sampler1Location; }
	GLuint GetWVP() const { return WVPLocation; }

	void LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);
	void Cleanup();

private:
	void CreateShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
	GLuint LoadShaderFile(const char* filePath, GLenum type);
	void LoadAttributes();
	void EvaluateShader(int infoLength, GLuint id);

	GLuint programID = 0;
	GLuint verticesLocation = 0;
	GLuint colorsLocation = 0;
	GLuint textureCoordsLocation = 0;
	GLuint sampler1Location = 0;
	GLuint WVPLocation = 0;
	GLint result = GL_FALSE;
	int infoLogLength = 0;
};

#endif // !SHADER_H