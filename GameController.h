#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"

class GameController : public Singleton<GameController>
{
public:
	GameController() = default;
	virtual ~GameController() {}

	void Initialize();
	void RunGame();

private:
	Camera camera;
	Shader diffuseShader = {};
	Shader colorShader = {};
	std::vector<Mesh*> boxMeshes;
	Mesh* lightMesh = {};

	static glm::vec3 GetRandomPosition(float minLength, float maxLength)
	{
		float scalar = GetRandomFloat(minLength, maxLength);
		float angle = GetRandomFloat(0.f, glm::radians(360.f));
		glm::vec3 direction(sin(angle), cos(angle), 0.f);

		return scalar * direction;
	}

	static float GetRandomFloat(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
};

#endif // !GAME_CONTROLLER_H