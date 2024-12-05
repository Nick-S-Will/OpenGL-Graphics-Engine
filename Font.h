#pragma once
#ifndef FONT_H
#define FONT_H

#include "StandardIncludes.h"

class Shader;

struct Character
{
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font
{
public:
	void Create(Shader* shader, std::string name, FT_UInt size);
	void RenderText(std::string text, float x, float y, glm::vec3 color, float scale = 1.f);

private:
	Shader* shader = nullptr;
	FT_Library library = nullptr;
	FT_Face face = nullptr;
	std::map<char, Character> characters;
	GLuint vertexBuffer = 0;
	glm::mat4 orthographicProjection = {};

	void Initialize(std::string fileName, FT_UInt size);
	void CreateCharacters();
	void AllocateBuffers();
};

#endif // !FONT_H