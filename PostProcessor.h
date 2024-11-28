#pragma once
#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "StandardIncludes.h"

class Shader;

enum class EffectType : int {
	None = 0,
	Inverted = 1,
	Grayscale = 2,
	Blur = 3,
	Sharp = 4,
	Edge = 5,
	Rainbow = 6
};

constexpr int EffectTypeCount = 7;

class PostProcessor
{
public:
	EffectType effectType;

	std::string GetEffectName() const
	{
		switch (effectType)
		{
		case EffectType::Inverted: return "Inverted";
		case EffectType::Grayscale: return "Grayscale";
		case EffectType::Blur: return "Blur";
		case EffectType::Sharp: return "Sharp";
		case EffectType::Edge: return "Edge Detection";
		case EffectType::Rainbow: return "Rainbow";
		default: return "None";
		}
	}

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