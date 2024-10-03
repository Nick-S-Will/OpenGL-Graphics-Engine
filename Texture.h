#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "StandardIncludes.h"

class Texture
{
public:
	virtual ~Texture();

	GLuint GetTexture() const { return texture; }

	void LoadTexture(std::string fileName, GLenum wrapMode);
	void CleanUp();

private:
	int width = -1;
	int height = -1;
	int channelCount = -1;
	GLuint texture = 0;
};

#endif // !TEXTURE_H