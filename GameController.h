#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#define LIGHT_COUNT 1

#include "StandardIncludes.h"
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "Skybox.h"
#include "PostProcessor.h"

enum class GameMode : int {
	MoveLight,
	Transform,
	WaterScene,
	SpaceScene
};

class GameController : public Singleton<GameController>
{
public:
	GameMode gameMode = GameMode::MoveLight;
	float specularStrength = 1.f;
	glm::vec3 specularColor = { 1.f, 1.f, 1.f };

	GameController() = default;
	virtual ~GameController() {}

	void Initialize();
	void RunGame();
	void ResetLightPosition() { lightMeshes[0]->position = glm::vec3(0.f); }

private:
	Camera camera;
	PostProcessor postProcessor = {};
	Shader diffuseShader = {};
	Shader colorShader = {};
	Shader fontShader = {};
	Shader skyboxShader = {};
	Shader postProcessorShader = {};
	std::vector<Mesh*> meshes;
	std::vector<Mesh*> lightMeshes;
	Skybox* skybox = nullptr;
	GLuint vao;

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