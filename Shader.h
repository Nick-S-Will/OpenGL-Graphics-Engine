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
	GLuint GetTangents() { return tangentsLocation; }
	GLuint GetBitangents() { return bitangentsLocation; }
	GLuint GetTextureCoords() { return textureCoordsLocation; }

	void LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);
	void SetMat4(const char* name, glm::mat4 value);
	void SetVec3(const char* name, glm::vec3 value);
	void SetFloat(const char* name, float value);
	void SetInt(const char* name, int value);
	void SetTextureSampler(const char* name, GLuint texUint, int texUintId, int value);
	void SetArrayVec3(const std::string arrayName, int index, const std::string fieldName, glm::vec3 value);
	void SetArrayFloat(const std::string arrayName, int index, const std::string fieldName, float value);
	void SetArrayInt(const std::string arrayName, int index, const std::string fieldName, int value);
	void Cleanup();

private:
	void CreateShaderProgram(const char* vertexFilePath, const char* fragmentFilePath);
	GLuint LoadShaderFile(const char* filePath, GLenum type);
	void LoadAttributes();
	void EvaluateShader(int infoLength, GLuint id);

	GLuint programID = 0;
	GLuint verticesLocation = 0;
	GLuint normalsLocation = 0;
	GLuint tangentsLocation = 0;
	GLuint bitangentsLocation = 0;
	GLuint textureCoordsLocation = 0;
	GLint result = GL_FALSE;
	int infoLogLength = 0;
};

#endif // !SHADER_H