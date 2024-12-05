#pragma once
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#define LIGHT_COUNT 1

#include "StandardIncludes.h"
#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"
#include "Mesh.h"
#include "Font.h"
#include "PostProcessor.h"
#include "WindowController.h"

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
	void ResetLightPosition() { lightMeshes[0]->position = lightPosition; }

private:
	Camera camera;
	Shader skyboxShader = {};
	Skybox* skybox = nullptr;
	Shader diffuseShader = {};
	std::vector<Mesh*> meshes;
	Shader colorShader = {};
	std::vector<Mesh*> lightMeshes;
	glm::vec3 lightPosition = glm::vec3(0.f, 0.f, 15.f);
	Shader fontShader = {};
	Font* arialFont = nullptr;
	int fontSize = 20;
	Shader postProcessorShader = {};
	PostProcessor postProcessor = {};
	GLuint vao;

	void LoadAssets();
	void CleanupAssets();
	void UpdateMoveLightScene();
	void UpdateTransformScene();

	void RenderLines(const std::vector<std::string>& lines, const glm::vec3& color)
	{
		for (int i = 0; i < (int)lines.size(); i++)
		{
			arialFont->RenderText(lines[i], fontSize / 4.f, (i + 1) * fontSize, color);
		}
	}

	static glm::vec3 GetMouseDelta()
	{
		GLFWwindow* window = WindowController::GetInstance().GetWindow();
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			int windowWidth, windowHeight;
			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			return glm::vec3(2. * mouseX / windowWidth - 1., 1. - (2. * mouseY) / windowHeight, 0);
		}

		return glm::vec3(0.f);
	}

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

	static std::string Vec3ToString(glm::vec3 vec3)
	{
		return "(" + std::to_string(vec3.x) + ", " + std::to_string(vec3.y) + ", " + std::to_string(vec3.z) + ")";
	}
};

#endif // !GAME_CONTROLLER_H