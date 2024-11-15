#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
	virtual ~Skybox();

	void Create(Shader* shader, std::string file, std::vector<std::string> faces);
	void Cleanup();
	void Render(glm::mat4 pv);

private:
	void SetShaderVariables(glm::mat4 pv);
	void BindAttributes();

	Shader* shader = nullptr;
	Texture texture = {};
	GLuint vertexBuffer = 0;
	std::vector<GLfloat> vertexData;
};

#endif // !SKYBOX_H