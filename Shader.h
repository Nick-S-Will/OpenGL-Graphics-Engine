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
	GLuint GetNormals() const { return normalsLocation; }
	GLuint GetTextureCoords() { return textureCoordsLocation; }
	GLuint GetSampler1() { return sampler1Location; }
	GLuint GetSampler2() { return sampler2Location; }

	void LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);
	void SetVec3(const char* name, glm::vec3 value);
	void SetMat4(const char* name, glm::mat4 value);
	void SetFloat(const char* name, float value);
	void Cleanup();

private:
	void CreateShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
	GLuint LoadShaderFile(const char* filePath, GLenum type);
	void LoadAttributes();
	void EvaluateShader(int infoLength, GLuint id);

	GLuint programID = 0;
	GLuint verticesLocation = 0;
	GLuint normalsLocation = 0;
	GLuint textureCoordsLocation = 0;
	GLuint sampler1Location = 0;
	GLuint sampler2Location = 0;
	GLint result = GL_FALSE;
	int infoLogLength = 0;
};

#endif // !SHADER_H